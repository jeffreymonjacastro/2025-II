#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv) {
  int numtasks, rank, next, prev, tag1 = 1, tag2 = 2, N = 1 << 22;
  int sbuf[N], rbuf[N];
  double t0, t1;

  MPI_Status status;
  MPI_Request request;
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

  // Invierta el orden de Send/Recv, ¿qué sucede?
  // MPI_Recv(rbuf, N, MPI_INT, prev, tag1, MPI_COMM_WORLD, &status);
  // MPI_Send(sbuf, N, MPI_INT, next, tag1, MPI_COMM_WORLD);

  // Pruebe el recibo desbloqueado
  MPI_Irecv(rbuf, N, MPI_INT, prev, tag1, MPI_COMM_WORLD, &request);
  MPI_Send(sbuf, N, MPI_INT, next, tag1, MPI_COMM_WORLD);

  // El wait esta despues del send para que el proceso pueda seguir trabajando. Si se pone junto al Irecv, el proceso se bloquea hasta que llegue el mensaje.
  MPI_Wait(&request, &status);

  t1 = MPI_Wtime();

  if (rank == 0)
    cout << "Tiempo (s): " << t1 - t0 << endl;

  MPI_Finalize();
}
