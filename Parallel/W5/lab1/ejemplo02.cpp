// Ejemplo de comunicacion punto a punto
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv) {
  int rank, size, numero = 0, arr_numero[10], count;

  MPI_Status estado;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  auto one_variable = [&]() {
    // 2. Imprima un mensaje indicando el dato que
    // Envia el proceso 0 al 1, y un mensaje desde el proceso 1 cuando reciba el dato
    if (rank == 0) {
      numero = 1000;

      // Se debe pasar la direccion de memoria del dato a enviar
      MPI_Send(&numero, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      cout << "Imprimiendo desde el rank " << rank << " que envia el numero " << numero << endl;
    } else if (rank == 1) {
      // El tipo que se envia es el mismo del que se recibe
      MPI_Recv(&numero, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &estado);
      cout << "Imprimiendo desde el rank " << rank << " que recibe el numero " << numero << endl;
    }
  };

  auto array_variable = [&]() {
    // 3. Envie un array

    if (rank == 0) {
      arr_numero[4] = 8;
      // La direccion del array es el nombre del array
      MPI_Send(arr_numero, 10, MPI_INT, 1, 999, MPI_COMM_WORLD);
      cout << "Imprimiendo desde el rank " << rank << " que envia el numero " << *(arr_numero + 4) << endl;
    } else if (rank == 1) {
      MPI_Recv(arr_numero, 10, MPI_INT, 0, 999, MPI_COMM_WORLD, &estado);
      cout << "Imprimiendo desde el rank " << rank << " que recibe el numero " << *(arr_numero + 4) << endl;

      // Imprima atributos de MPI_Status
      MPI_Get_count(&estado, MPI_INT, &count);
      cout << "estado.MPI_SOURCE: " << estado.MPI_SOURCE << endl;
      cout << "estado.MPI_TAG: " << estado.MPI_TAG << endl;
      cout << "cantidad de elementos: " << count << endl;
    }
  };

  // 1. Incluya un MPI_Abort si se ejecuta el codigo en mas de dos procesos
  if (size > 2) {
    if (rank == 0) {
      // Todos los procesos se deben abortar
      MPI_Abort(MPI_COMM_WORLD, -1);

      cout << "Error: Este programa requiere exactamente 2 procesos, se proporcionaron " << size << endl;
    }
  } else {
    // one_variable();
    array_variable();
  }

  MPI_Finalize();
}
