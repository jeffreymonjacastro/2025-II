#ifdef _OPENMP
	#include <omp.h>
#endif
#include <stdio.h>

int work(int i) {
  return i;
}

int main() {
  int n = 1 << 20, mx = 0, pmax, r;

	#ifdef _OPENMP
		double t1 = omp_get_wtime();
	#endif

	#pragma omp parallel for reduction(max : mx) 
	for (int i = 0; i < n; i++) {
		r = work(i);
		mx = (r > mx ? r : mx);
	}

	#ifdef _OPENMP
		double t2 = omp_get_wtime() - t1;
		printf("\ntiempo: %f\n", t2);
	#endif

	printf("max: %i \n", mx);
}
