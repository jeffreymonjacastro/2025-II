/****************************************************************
 2 enteros recorren un anillo y se suman sus valores en cada proceso
Input: ninguno
Output: suma total acumulada de cada uno de los dos enteros
de cada proceso  
Nota: puede ejecutarse en cualquier numero de procesos
*                                                              *
 ****************************************************************/
#include <iostream>
#include <mpi.h>

int main (int argc, char *argv[])
{
  int my_rank, size;
  int i, right, left;

  struct buff{
     int   i;
     int   j;
  } snd_buf, rcv_buf, sum;

// 1. Declare el tipo MPI
  MPI_Datatype mpi_recv_send;

  MPI_Status  status;
  MPI_Request request;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  right = (my_rank+1)      % size;
  left  = (my_rank-1+size) % size;

// 2. defina el tipo MPI para envio y recibo
//    de sumas parciales
  MPI_Type_contiguous(2,MPI_INT,&mpi_recv_send); 
  MPI_Type_commit(&mpi_recv_send);

  sum.i = 0;            sum.j = 0;
  snd_buf.i = my_rank;  snd_buf.j = 10*my_rank; // Step 1 = init

// 3. replique recorrido de anillo no bloqueado (como en PD3) 
  for( i = 0; i < size; i++) 
  {
    MPI_Issend(&snd_buf,1, mpi_recv_send,right,9,MPI_COMM_WORLD,&request);
    MPI_Recv(&rcv_buf,1,mpi_recv_send,left,9,MPI_COMM_WORLD,&status);
    MPI_Wait(&request,&status);

    snd_buf = rcv_buf;                       
    sum.i += rcv_buf.i;  sum.j += rcv_buf.j; 
  }

  printf ("PE%i:\tSum = %i\t%i\n", my_rank, sum.i, sum.j);

// 4. Libere memoria
  MPI_Type_free(&mpi_recv_send);
  
MPI_Finalize();
}
