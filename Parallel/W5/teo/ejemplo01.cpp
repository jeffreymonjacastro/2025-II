// Encabezados
#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[]) {
  // Declaracion de variables
  int ierr, rank, size, dato = 10;

  // Esta parte es estandar para los ejercicios de clase
  // Para iniciar el programa
  ierr = MPI_Init(&argc, &argv);

  // Se pasa la variable por referencia
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  //  2. Modifica el valor de datos solo para que el proceso 2 tenga dato = 20
  if (rank == 2)
    dato = 20;

  // 1. Ejecuta la impresion solo si MPI_Init resulto (use MPI_SUCCESS)
  if (ierr == MPI_SUCCESS)
    printf("Mensaje del proceso %d, con dato %d, de un total de %d procesos \n", rank, dato, size);

  MPI_Finalize();
}
