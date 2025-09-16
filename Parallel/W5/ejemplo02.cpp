// Ejemplo de comunicacion punto a punto
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv) {
  int rank, size, numero[10], count;
  MPI_Status estado;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // 1. incluya un MPI_Abort si se ejecuta el codigo en mas de dos procesos
  if (size != 2) {
    if (rank == 0) {
      cout << "Error: Este programa requiere exactamente 2 procesos, se proporcionaron " << size << endl;
    }
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  // 2. imprima un mensaje indicando el dato que
  // envia el proceso 0 al 1, y un mensaje desde el proceso 1 cuando reciba el dato
  if (rank == 0) {
    numero[0] = 10;
    numero[1] = 20;
    MPI_Send(numero, 2, MPI_INT, 1, 100, MPI_COMM_WORLD);
    cout << "Imprimiendo desde el rank " << rank << " que envia los numeros " << numero[0] << "\t" << numero[1] << endl;
  } else if (rank == 1) {
    MPI_Recv(numero, 2, MPI_INT, 0, 100, MPI_COMM_WORLD, &estado);
    cout << "Imprimiendo desde el rank " << rank << " que recibe los numeros " << numero[0] << "\t" << numero[1] << endl;
  }

  // 3. Envie un array

  if (rank == 0) {
    numero[4] = 8;
    MPI_Send(numero, 10, MPI_INT, 1, 999, MPI_COMM_WORLD);
    cout << "Imprimiendo desde el rank " << rank << " que envia el numero " << *(numero + 4) << endl;
  } else if (rank == 1) {
    MPI_Recv(numero, 10, MPI_INT, 0, 999, MPI_COMM_WORLD, &estado);
    cout << "Imprimiendo desde el rank " << rank << " que recibe el numero " << *(numero + 4) << endl;

    // Imprima atributos de MPI_Status
    MPI_Get_count(&estado, MPI_INT, &count);
    cout << "estado.MPI_SOURCE: " << estado.MPI_SOURCE << endl;
    cout << "estado.MPI_TAG: " << estado.MPI_TAG << endl;
    cout << "cantidad de elementos: " << count << endl;
  }

  MPI_Finalize();
}
