#include <iostream>
#include <mpi.h>
#include <cstdio>

using namespace std;

int main(int argc, char *argv[]) {
  int rank, size, n = 5;
  string msg;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status state;

  auto ping_pong = [&]() {
    for (int i = 0; i < n; i++) {
      if (rank == 0) {
        msg = "ping " + to_string(i);
        MPI_Send(&msg[0], 7 , MPI_CHAR, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&msg[0], 7, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &state);
        printf("Process %d received: %s\n", rank, msg.c_str());
      } else {
        MPI_Recv(&msg[0], 7, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &state);
        printf("Process %d received: %s\n", rank, msg.c_str());

        msg = "pong " + to_string(i);
        MPI_Send(&msg[0], 7, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
      }
    }
  };


  if (size > 2) {
    if (rank == 0) {
      printf("Just two processes are allowed, you enter %d\n", size);
      MPI_Abort(MPI_COMM_WORLD, -1);
    }
  } else {
    ping_pong();
  }


  MPI_Finalize();
}