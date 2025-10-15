#include <iostream>
#include <omp.h>

int main() {
  int i, n = 32, chunk = 10;
  float a[n], b[n], result = 0.0;

  for (i = 0; i < n; i++) {
    a[i] = 1.0;
    b[i] = 2.0;
  }

  //* Puede ser dynamic o static
  //* static: cada hilo recibe un chunk de iteraciones de tamaño fijo
  //* dynamic: cada hilo recibe una cantidad variable de iteraciones dependiendo de la carga
  #pragma omp parallel for schedule(static) reduction(+ : result) // num_threads(4) private(i)
  for (i = 0; i < n; i++) {
    result += (a[i] * b[i]);
    printf("Resultado parcial= %f en i= %i del hilo %i\n", result, i, omp_get_thread_num());
  }
  printf("Producto escalar= %f\n", result);
}
