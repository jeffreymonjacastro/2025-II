#include <iostream>
#include <mpi.h>

int main (int argc, char *argv[])
{
  int my_rank, size;
  int snd_buf, rcv_buf;
  int right, left;
  int sum, i;
  MPI_Status  status;
  MPI_Request request;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

//  ------ solucion para Allreduce -----
  right = (my_rank+1)      % size;
  left  = (my_rank-1+size) % size;
  sum = 0;
  snd_buf = my_rank;
double t1=MPI_Wtime();
MPI_Allreduce (&my_rank, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
/*
  for( i = 0; i < size; i++) 
  {
//    MPI_Isend(&snd_buf, 1, MPI_INT, right, 17, MPI_COMM_WORLD, &request);
    MPI_Send(&snd_buf, 1, MPI_INT, right, 17, MPI_COMM_WORLD);
    MPI_Recv  (&rcv_buf, 1, MPI_INT, left,  17, MPI_COMM_WORLD, &status);
//    MPI_Wait(&request, &status);
    snd_buf = rcv_buf;
    sum += rcv_buf;
  }
*/
double t2=MPI_Wtime();
 // ------ con una directiva de comunicacion colectiva ---
if(my_rank==0)
  printf ("PE%i:\tSum = %i time: %f\n", my_rank, sum,t2-t1);

  MPI_Finalize();
}
