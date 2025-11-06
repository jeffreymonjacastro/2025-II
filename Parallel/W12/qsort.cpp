#include <iostream>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;
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

void qs(int *z, int zstart, int zend, int firstcall){
  {int pivot;
    if(firstcall == 1) {
      qs(z,0,zend,0);
    } else {
      if(zstart<zend) {
        pivot = separate(z, zstart,zend);
        qs(z, zstart, pivot-1,0);
        qs(z,pivot+1,zend,0);
      }
    }
  }
}

int main(){
  int i,n = 100000;//,*w=new int[n];
  int w[n];
  int w_backup[n];
  double total_time = 0.0;
  const int NUM_RUNS = 5;
  
  // Generar array original una sola vez
  for(i = 0; i<n; i++) w_backup[i] = rand()%100;
  // generar_array(w,n);
  // cout<<"array original: "; imprimir_array(w,n);
  
  cout << "Versión secuencial (sin OpenMP)\n";
  cout << "Ejecutando quicksort " << NUM_RUNS << " veces...\n";
  cout << "========================================\n";
  
  // Ejecutar quicksort 5 veces
  for(int run = 0; run < NUM_RUNS; run++) {
    // Copiar array original para cada ejecución
    for(i = 0; i < n; i++) w[i] = w_backup[i];
    
    // Iniciar el cronómetro
    auto start = high_resolution_clock::now();
    
    qs(w,0,n-1,1);
    
    // Detener el cronómetro
    auto stop = high_resolution_clock::now();
    duration<double> secs = duration_cast<duration<double>>(stop - start);
    
    total_time += secs.count();
    cout << "Ejecución " << (run + 1) << ": " << secs.count() << " segundos" << endl;
  }
  
  // cout<<"array ordenada: "; imprimir_array(w,n);
  
  double avg_time = total_time / NUM_RUNS;
  cout << "========================================\n";
  cout << "Tiempo promedio: " << avg_time << " segundos" << endl;
  cout << "Tiempo total: " << total_time << " segundos" << endl;

  return 0;
} 