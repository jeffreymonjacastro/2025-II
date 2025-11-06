#include <iostream>
#include <iomanip>

#ifdef _OPENMP
  #include <omp.h>
#endif

using namespace std;

void generar_array(int* w, int &n){
  cout<<"ingrese el numero de enteros a ordenar: "<<endl;
  cin>>n;
  for(int i=0; i<n; i++) w[i] = rand()%100;
}

void imprimir_array(int* w, int n){
  for(int i=0;i<n;i++) cout<<setw(5)<<w[i];
  cout<<endl;
}

void swap(int *x, int *y){
  int temp = *x;
  *x = *y;
  *y = temp;
}

int separate(int *x, int low, int high){
  int i, pivot, last;
  pivot = x[low]; 
  swap(x+low, x+high);
  last = low;

  for(i = low; i<high; i++){
    if(x[i] <= pivot){
      swap(x+last, x+i);
      last += 1;
    }
  }

  swap(x+last, x+high);
  return last;
}

void qs(int *z, int zstart, int zend){
  if(zstart < zend) {
    int pivot = separate(z, zstart, zend);
    
    // Evito ejecutar en paralelo subproblemas pequeños 
    if(zend - zstart > 1000) {
      #pragma omp task shared(z) firstprivate(zstart, pivot)
      qs(z, zstart, pivot-1);
      
      #pragma omp task shared(z) firstprivate(pivot, zend)
      qs(z, pivot+1, zend);
      
      #pragma omp taskwait
    } else {
      qs(z, zstart, pivot-1);
      qs(z, pivot+1, zend);
    }
  }
}

int main(){
  int n = 100000;  
  int *w = new int[n];
  int *w_backup = new int[n];
  double secs = 0.0;
  double total_time = 0.0;
  int num_threads = 1;
  const int NUM_RUNS = 5;

  // Generar array original una sola vez
  for(int i=0; i<n; i++) w_backup[i] = rand()%100;
  
  // generar_array(w,n);
  // imprimir_array(w,n);

  #ifdef _OPENMP
    num_threads = omp_get_max_threads();
  #endif
  
  cout << "Usando OpenMP con " << num_threads << " threads\n";
  cout << "Ejecutando quicksort " << NUM_RUNS << " veces...\n";
  cout << "========================================\n";

  // Ejecutar quicksort 5 veces
  for(int run = 0; run < NUM_RUNS; run++) {
    // Copiar array original para cada ejecución
    for(int i=0; i<n; i++) w[i] = w_backup[i];
    
    #ifdef _OPENMP
      double t0 = omp_get_wtime();
    
      #pragma omp parallel 
      {
        #pragma omp single
        {
          qs(w,0,n-1);
        }
      }
      
      double t1 = omp_get_wtime();
      secs = t1 - t0;
      total_time += secs;
      cout << "Ejecución " << (run + 1) << ": " << secs << " segundos" << endl;
    #endif
  }

  // imprimir_array(w,n);

  #ifdef _OPENMP
    double avg_time = total_time / NUM_RUNS;
    cout << "========================================\n";
    cout << "Tiempo promedio: " << avg_time << " segundos" << endl;
    cout << "Tiempo total: " << total_time << " segundos" << endl;
  #endif

  delete[] w;
  delete[] w_backup;
  return 0;
} 
