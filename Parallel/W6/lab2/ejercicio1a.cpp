#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  int rank, size;
  const int N = 50;
  double start_time, end_time, elapsed_time;
  int ping_pong_message = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 2) {
    if (rank == 0) {
      cout << "Este programa requiere exactamente dos procesos para funcionar. Saliendo..." << endl;
    }
    MPI_Finalize();
    return 1;
  }

  // med time
  start_time = MPI_Wtime();

  for (int i = 0; i < N; ++i) {
    if (rank == 0) {
      // p0 envia el ping
      MPI_Send(&ping_pong_message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      // p0 recibe el pong
      MPI_Recv(&ping_pong_message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else { // rank == 1
      // p1 recibe el ping
      MPI_Recv(&ping_pong_message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // p1 envia el pong
      MPI_Send(&ping_pong_message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }

  end_time = MPI_Wtime();

  if (rank == 0) {
    elapsed_time = (end_time - start_time) / (N * 2.0); // Dividimos por 2 para el tiempo de un solo "viaje"
    double elapsed_microseconds = elapsed_time * 1e6;
    cout << "Tiempo de comunicacion por mensaje: " << elapsed_microseconds << " microsegundos" << endl;
  }

  MPI_Finalize();
  return 0;
}