/*  MPI_Allreduce  */

#include <iostream>
#include <mpi.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[]) {
  int me, numprocs, sum;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &me);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  // 1. Envie el rank de cada proceso al maestro y sume los valores en cada proceso
  //! MPI_Allreduce(&sendbuf, &recvbuf, count, datatype, op, comm)
  MPI_Allreduce(&me, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  // 2. Imprima el resultado desde cada proceso
  printf("proceso %i: sum = %i\n", me, sum);

  MPI_Finalize();
}
