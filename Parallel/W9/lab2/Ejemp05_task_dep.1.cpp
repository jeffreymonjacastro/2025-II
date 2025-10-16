/*
 * @@name:	task_dep.1c
 * @@type:	C
 * @@compilable:	yes
 * @@linkable:	yes
 * @@expect:	success
 */
#include <iostream>
#include <omp.h>
int main() {
  int x = 1;

	#pragma omp parallel

	#pragma omp single
  {
		// La variable x (out) es compartida entre las tareas de manera secuencial
		#pragma omp task
    x = 2;

		// Espera a que la tarea anterior haya terminado para recibir x (in)
		#pragma omp task shared(x) depend(in : x)
    printf("x = %d\n", x);
  }

  return 0;
}
