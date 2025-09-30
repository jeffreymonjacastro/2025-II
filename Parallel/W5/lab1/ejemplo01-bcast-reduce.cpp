#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
  int rank, size, data, resultado;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0)
    data = 5;

  printf("Proceso %d tiene data= %d\n", rank, data);

  // 1. Envie data desde el maestro al resto de procesos
  //! MPI_Bcast(&buff, count, datatype, root, comm)
  MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // 2. Modifique el valor de data en cada proceso
  data *= rank;

  // 3. Recolecte (reduce) los resultados de cada proceso en el maestro y sumelos en la variable resultado
  // MPI_Reduce(&sendbuf, &recvbuf, count, datatype, op, root, comm)
  MPI_Reduce(&data, &resultado, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  // 4. Imprima el resultado desde el maestro
  if (rank == 0)
    cout << "Proceso " << rank << ", data: " << data << ", resultado: " << resultado << endl;

  MPI_Finalize();
}
