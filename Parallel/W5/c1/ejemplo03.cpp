#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv) {
  int rank, size, numero, conteo;
  MPI_Status estado;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // 1. Como en el ejemplo02, pero realice el envío
  // de datos del maestro a todos los procesos (Broadcast)

  if (rank == 0) {
    numero = 4;
    // envio al resto de procesos
    for (int i = 1; i < size; i++)
      MPI_Send(&numero, 1, MPI_INT, i, 999, MPI_COMM_WORLD);

    cout << "Imprimiendo desde el rank " << rank << " que envia el numero " << numero << endl;
  } else {
    // Recibo desde el maestro
    MPI_Recv(&numero, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
    cout << "Imprimiendo desde el rank " << rank << " que recibe el numero " << numero << endl;
  }

  MPI_Finalize();
}
