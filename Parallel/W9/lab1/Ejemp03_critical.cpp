#ifdef _OPENMP
	#include <omp.h>
#endif
#include <stdio.h>

int work(int i) {
  return i;
}

int main() {
  int n = 1 << 10, mx = 0, pmax, r;

	#ifdef _OPENMP
		double t1 = omp_get_wtime();
	#endif

	// paralelice el calculo del r maximo
	// evaluado en work()
	// Evalue el uso de critical dentro y fuera
	// del for loop
	#pragma omp parallel private(pmax)
  {
    pmax = 0;

		//* Cada hilo calcula su maximo local O(n/p)
		#pragma omp for
    for (int i = 0; i < n; i++) {
      r = work(i);
      pmax = (r > pmax ? r : pmax);
    }

		//* Luego, se actualiza el maximo global O(p)
    #pragma omp critical
    {
      mx = (pmax > mx ? pmax : mx);
    }
  }

	#ifdef _OPENMP
		double t2 = omp_get_wtime() - t1;
		printf("\ntiempo: %f\n", t2);
	#endif

	printf("max: %i \n", mx);
}
