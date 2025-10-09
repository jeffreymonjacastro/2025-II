#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int N = 1 << 25;              // application-related data
  int my_rank, num_procs, rank; // MPI-related data
  double *result_local = new double[N]{0};
  double *result_array = nullptr;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  MPI_Request request;
  MPI_Status status;

  // calculo en cada proceso
  for (int i = 0; i < N; i++)
    result_local[i] = 100.0 + 1.0 * my_rank;

  if (my_rank == 0) {
    result_array = new double[N * num_procs]{0};
  }

  double t1 = MPI_Wtime();
  // Solucion para
  MPI_Gather(result_local, N, MPI_DOUBLE, result_array, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  /*
    if (my_rank != 0)  //  todos menos el maestro
    { // envio desde todos al maestro
      MPI_Isend(result_local, N, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD,&request);
  //    MPI_Wait(&request, &status);
  //    MPI_Send(result_local, N, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);
    } else  // solo en el maestro
    {
      for (rank=1; rank<num_procs; rank++)
      { // resultado de procesos 1, 2, ...
        MPI_Recv(&result_array[N*rank], N, MPI_DOUBLE, rank, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      }
    }
  if (my_rank != 0) MPI_Wait(&request, &status);
  */
  double t2 = MPI_Wtime();

  if (my_rank == 0) { // imprime un elemento por proceso para control

    for (rank = 0; rank < num_procs; rank++)
      printf("Resultado desde el proceso %i es %f \n", rank, result_array[N * rank]);
    printf("PE: %i time: %f\n", my_rank, t2 - t1);
  }

  MPI_Finalize();
}
