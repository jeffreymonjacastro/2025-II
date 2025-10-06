#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
  int mpi_rank, mpi_size;
  MPI_Status stat;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  double t0 = 0.0, t1 = 0.0, t2 = 0.0, t3 = 0.0, t4 = 0.0, tf = 0.0;

  // const int array_size = (int)pow(10,6);;
  const int array_size = (int)pow(2, 20);
  const int subpr_size = array_size / mpi_size;
  int send_arr[array_size], recv_arr[subpr_size];
  double result = 0.0, local_sum = 0.0, global_sum[mpi_size];

  // inicializar en el maestro, array con elementos entre 0-100
  if (mpi_rank == 0) {

    for (int i = 0; i < mpi_size; ++i)
      global_sum[i] = 0.0;

    // srand((int)time(0));
    srand(123456);

    for (int i = 0; i < array_size; ++i)
      send_arr[i] = rand() % 101;
  }

  t0 = MPI_Wtime();

  MPI_Scatter(send_arr, subpr_size, MPI_INT, &recv_arr, subpr_size, MPI_INT, 0, MPI_COMM_WORLD);

  t1 = MPI_Wtime();
  // Promedios locales
  for (int i = 0; i < subpr_size; i++)
    local_sum += recv_arr[i];
  local_sum /= subpr_size;

  t2 = MPI_Wtime();
  MPI_Gather(&local_sum, 1, MPI_DOUBLE, global_sum, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  t3 = MPI_Wtime();

  // promedio global y tiempos en el maestro
  if (mpi_rank == 0) {
    for (int i = 0; i < mpi_size; i++)
      result += global_sum[i];
    result /= mpi_size;
    tf = MPI_Wtime();

    printf("Promedio resultado: %1.4f \n", result);
    printf("Tiempo Scatter: %1.6f ms\n", 1000 * (t1 - t0));
    printf("Tiempo Gather: %1.6f ms\n", 1000 * (t3 - t2));
    printf("Tiempo communicacion: %1.6f ms\n", 1000 * ((t1 - t0) + (t3 - t2)));
    printf("Tiempo computo (suma): %1.6f ms\n", 1000 * (t2 - t1));
    printf("Tiempo total: %1.6f ms\n", 1000 * (tf - t0));
  }

  MPI_Finalize();
}
