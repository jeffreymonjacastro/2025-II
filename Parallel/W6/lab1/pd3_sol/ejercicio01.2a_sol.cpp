#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int my_rank;
  float temp, mass;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (my_rank == 0) {
    temp = 100;
    mass = 2.5;
    MPI_Send(&temp, 1, MPI_FLOAT, 1, 17, MPI_COMM_WORLD);
    MPI_Send(&mass, 1, MPI_FLOAT, 1, 18, MPI_COMM_WORLD);
  } else if (my_rank == 1) {
    MPI_Recv(&mass, 1, MPI_FLOAT, 0, 18, MPI_COMM_WORLD, &status);
    printf("I am process %i and received mass=%3.1f \n", my_rank, mass);
    MPI_Recv(&temp, 1, MPI_FLOAT, 0, 17, MPI_COMM_WORLD, &status);
    printf("I am process %i and received temp=%3.0f \n", my_rank, temp);
  }

  MPI_Finalize();
}
