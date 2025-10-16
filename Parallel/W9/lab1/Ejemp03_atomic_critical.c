// #include <iostream>
#include <stdio.h>
#include <omp.h>

static long num_steps = 1 << 20;
double step;

int main() {
  int i, nthreads = omp_get_num_threads();
  double pi = 0, sum[nthreads];
  step = 1.0 / (double)num_steps;
  double t1 = omp_get_wtime();

	// Utilice critical y atomic para el calculo de pi
	#pragma omp parallel
  {
    int i, id, nthrds;
    double x, sum;
    id = omp_get_thread_num();
    nthrds = omp_get_num_threads();

    printf("%d \n", nthreads);

    for (i = id; i < num_steps; i += nthrds) {
      x = (i + 0.5) * step;
      sum += 4.0 / (1.0 + x * x);
      // printf("id: %i, step: %f sum: %f\n",id,step,sum[id]);
    }

		#pragma omp atomic
		// #pragma omp critical
    pi += sum * step;

    printf("---id: %i sum: %f\n", id, sum);
  }

  printf("pi: %.12f ", pi);

  double t2 = omp_get_wtime() - t1;
  printf("\ntiempo: %f", t2);

  return 0;
}
