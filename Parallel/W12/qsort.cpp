#include <iostream>
#include <iomanip>
using namespace std;
void generar_array(int* w, int &n){
  cout<<"ingrese el numero de enteros a ordenar: "<<endl;
  cin>>n;
  for(int i=0; i<n; i++) w[i] = rand()%100;
}
void imprimir_array(int* w, int n){
  //for(int i=0;i<n;i++) cout<<setw(5)<<w[i];
  //cout<<endl;
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
  int i,n;//,*w=new int[n];
int w[n];
 for(i = 0; i<n; i++) w[i] = rand()%100;
//  generar_array(w,n);
  //cout<<"array original: "; imprimir_array(w,n);
  qs(w,0,n-1,1);
  //cout<<"array ordenada: "; imprimir_array(w,n);

  return 0;
} 
