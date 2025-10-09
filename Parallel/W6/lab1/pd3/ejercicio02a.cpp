#include <iostream>
#include <mpi.h>


int main (int argc, char *argv[])
{
  int my_rank, size;
  int sum;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  /* Calcula la suma de rank's */
  MPI_Allreduce (&my_rank, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD); 
 
  MPI_Finalize();
}
