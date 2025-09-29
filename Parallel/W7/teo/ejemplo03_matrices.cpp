#include <iostream>
#include <mpi.h>
#define N 8
#define M 4

void imprimir_matriz(double A[][M], double B[][M]) {
  printf("Matriz A\n");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      printf("%f ", A[i][j]);
    }
    printf("\n");
  }
  printf("Matriz B\n");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      printf("%f ", B[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  int p, rank, i, j, mb;
  double A[N][M], B[N][M], Al[N][M], Bl[N][M];

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
        A[i][j] = j;
        B[i][j] = 2 * j;
      }
    }
    printf("Matrices origen\n");
    imprimir_matriz(A, B);
  }

  // 1. Envie N/p filas a cada proceso, donde se almacenan
  //    submatrices Al, Bl de A, B
  mb = N / p;
  for (i = 0; i < mb; i++) {
    MPI_Scatter(A, M, MPI_DOUBLE, Al[i], M, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  for (i = 0; i < mb; i++) {
    for (j = 0; j < M; j++) {
      Al[i][j] *= 2;
      Bl[i][j] *= 2;
    }
  }

  // 2. Envie las submatrices desde cada proceso al maestro
  for (i = 0; i < mb; i++) {
    MPI_Gather(Al[i], M, MPI_DOUBLE, A[i * p], M, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }
  if (rank == 0) {
    printf("Matrices destino\n");
    imprimir_matriz(A, B);
  }

  MPI_Finalize();
}
