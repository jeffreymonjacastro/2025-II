#include <iostream>
#include <mpi.h>
#include <stdio.h>
using namespace std;

int main(int argc, char **argv) {
  int numtasks, rank, next, prev, tag1 = 1, tag2 = 2, N = 1 << 14;
  int sbuf[N], rbuf[N];
  double t0, t1;
  MPI_Status status;
  MPI_Init(&argc, &argv);                   // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);     // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Total number of processors

  prev = rank - 1;
  next = rank + 1;
  if (rank == 0)
    prev = numtasks - 1;
  if (rank == (numtasks - 1))
    next = 0;

  t0 = MPI_Wtime();

  // COMUNICACION CICLICA
  // 1. Envio sincronico
  // MPI_Ssend(sbuf,N,MPI_INT,next,tag1,MPI_COMM_WORLD);
  // MPI_Recv(rbuf,N,MPI_INT,prev,tag1,MPI_COMM_WORLD, &status);

  // Reserva espacio para el envio buffered
  int buffer_size = (MPI_BSEND_OVERHEAD + N * sizeof(int));
  void *buffer = malloc(buffer_size);
  MPI_Buffer_attach(buffer, buffer_size);

  // 2. Envio buffered
  MPI_Bsend(sbuf, N, MPI_INT, next, tag1, MPI_COMM_WORLD);
  MPI_Recv(rbuf, N, MPI_INT, prev, tag1, MPI_COMM_WORLD, &status);

  // Libera espacio reservado
  MPI_Buffer_detach(&buffer, &buffer_size);
  free(buffer);

  t1 = MPI_Wtime();

  if (rank == 0)
    cout << "Tiempo (s): " << t1 - t0 << endl;

  MPI_Finalize();
}
