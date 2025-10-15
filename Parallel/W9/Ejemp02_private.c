/*
 * @@name:	private.1c
 * @@type:	C
 * @@compilable:	yes
 * @@linkable:	yes
 * @@expect:	success
 */

// #include <iostream>
#include <stdio.h>
#include <omp.h>

int main() {
  int i, j;

  i = 1;
  j = 2;

  // paralelice el bloque y mida el efecto de
  // private, firsprivate

  // Caso 1: normal
  //* El pragma parallel tiene una barrera implícita al inicio y al final del bloque
  // #pragma omp parallel
  // {
  //   i = 3;
  //   j = j + 2;
  //   printf("parallel i: %i j: %i \n", i, j);
  // }

  // Caso 2: private
  //* El private crea una variable local para cada thread.
  //* En khipu, se inicializa con 0
  // #pragma omp parallel private(i,j)
  // {
  //   i = 3;
  //   j = j + 2;
  //   printf("parallel i: %i j: %i \n", i, j);
  // }

  // Caso 3: firstprivate
  //* El firstprivate crea una variable local para cada thread
  //* y la inicializa con el valor que tenia antes de entrar en el bloque paralelo
  #pragma omp parallel firstprivate(i, j)
  {
    i = 3;
    j = j + 2;
    printf("parallel i: %i j: %i \n", i, j);
  }

  // El master espera a que todos los threads acaben
  printf("i: %i j: %i \n", i, j);

  return 0;
}
