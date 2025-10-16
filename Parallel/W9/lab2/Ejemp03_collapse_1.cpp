/* 
 * Paralelismo de dos loops anidados
 */

#include <iostream>
#include <omp.h>

int main() {
  // const int N = 1 << 3, M = 1 << 3;
  const int N = 1<<2, M = 1<<4;
  int A[N][M];

	// Solo el primer for es paralelizado
	#pragma omp parallel for
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      A[i][j] = i + j;
      if (omp_get_thread_num() == 0)
        printf("Thread %d ejecuta (i=%d, j=%d)\n", omp_get_thread_num(), i, j);
    }
  }
}
