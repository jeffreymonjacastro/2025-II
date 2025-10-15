#include <omp.h>
#include <stdio.h>

// Global variables
int a = 0, b = 0, i, tid;
float x;

int main() {
  omp_set_num_threads(4);

  //* Es como el private pero se utiliza para varilables globales y estáticas 
  //* y mantiene el valor de la variable en el mismo hilo pero en diferentes regiones paralelas
  #pragma omp threadprivate(x)

  /* Explicitly turn off dynamic threads */
  //* Define cuantos hilos van a ocuparse en las regiones paralelas dependiendo de la carga y disponibilidad
  omp_set_dynamic(0);

  printf("1st Parallel Region:\n");

  #pragma omp parallel private(b, tid)
  {
    tid = omp_get_thread_num();
    a = tid;
    b = tid;
    x = 1.1 * tid + 1.0;
    printf("Thread %d:   a,b,x= %d %d %f\n", tid, a, b, x);
  } /* end of parallel section */

  printf("Master thread doing serial work here\n");
  printf("2nd Parallel Region:\n");

  #pragma omp parallel private(tid)
  {
    tid = omp_get_thread_num();
    printf("Thread %d:   a,b,x= %d %d %f\n", tid, a, b, x);
  }
}
