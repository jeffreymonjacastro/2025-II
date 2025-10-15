/*
 * @@name:	lastprivate.1c
 * @@type:	C
 * @@compilable:	yes
 * @@linkable:	no
 * @@expect:	success
 */

// #include <iostream>
#include <stdio.h>
#include <omp.h>

void lastpriv(int n, float *a, float *b) {
  int i = 5;

  // Paralelice el bloque y mida el efecto de lastprivate

  // Caso 1: sin lastprivate
  //* Fuera del for, el i es compartido y vale 5
  //* En los for, el iterador es privado por defecto
  // #pragma omp parallel 
  // {
  //   #pragma omp for 
  //   for (i = 0; i < n - 1; i++)
  //     a[i] = b[i] + b[i + 1];
  // }

  // Caso 2: con lastprivate
  //* El lastprivate hace que la variable i tome el valor de la ultima iteracion
  #pragma omp parallel
  {
    #pragma omp for lastprivate(i)
    for (i = 0; i < n - 1; i++)
      a[i] = b[i] + b[i + 1];
  }

  a[i] = b[i]; /* i == n-1*/
}

int main() {
  int i, n = 10;
  float a[n], b[n];

  for (i = 0; i < n; i++) {
    a[i] = i;
    b[i] = 2 * i;
    printf("ini: a_i: %f b_i %f\n", a[i], b[i]);
  }

  lastpriv(n, a, b);

  for (i = 0; i < n; i++)
    printf("a_i: %f b_i %f\n", a[i], b[i]);
}
