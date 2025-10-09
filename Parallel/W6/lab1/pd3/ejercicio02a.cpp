#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int my_rank, size;
  int sum = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status status;
  MPI_Request request;

  /* Calcula la suma de rank's sin utilizar Allreduce */
  // MPI_Allreduce(&my_rank, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  int send_b = my_rank;
  int recv_b;
  int rigth = (my_rank + 1) % size;
  int left = (my_rank - 1 + size) % size;

  double t1 = MPI_Wtime();

  for (int i = 0; i < size; i++) {
    // MPI_Send(&send_b, 1, MPI_INT, rigth, 1, MPI_COMM_WORLD);
    MPI_Isend(&send_b, 1, MPI_INT, rigth, 1, MPI_COMM_WORLD, &request);
    MPI_Recv(&recv_b, 1, MPI_INT, left, 1, MPI_COMM_WORLD, &status);
    MPI_Wait(&request, &status);

    send_b = recv_b;
    sum += recv_b;
  }

  double t2 = MPI_Wtime() - t1;

  if (my_rank == 0)
    printf("suma: %i, tiempo: %f\n", sum, t2);

  MPI_Finalize();
}
