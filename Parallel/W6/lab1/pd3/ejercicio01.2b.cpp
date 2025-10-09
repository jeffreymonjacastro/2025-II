#include <iostream>
#include <mpi.h>

#define LNG 500000

int main(int argc, char *argv[]) {
  int my_rank;
  float *temp, *mass;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  mass = (float *)malloc(sizeof(float) * LNG);
  temp = (float *)malloc(sizeof(float) * LNG);

  if (my_rank == 0) {
    temp[0] = 100;
    mass[0] = 2.5;
    MPI_Send(temp, LNG, MPI_FLOAT, 1, 17, MPI_COMM_WORLD);
    MPI_Send(mass, LNG, MPI_FLOAT, 1, 18, MPI_COMM_WORLD);
  } else if (my_rank == 1) {
    MPI_Recv(mass, LNG, MPI_FLOAT, 0, 18, MPI_COMM_WORLD, &status); // El tag estaba mal
    printf("I am process %i and received mass=%3.1f \n", my_rank, mass[0]);
    MPI_Recv(temp, LNG, MPI_FLOAT, 0, 17, MPI_COMM_WORLD, &status);
    printf("I am process %i and received temp=%3.0f \n", my_rank, temp[0]);
  }
  
  // Lo que pasa es que el MPI_Send varia dependiendo del tamaño del buffer. En este caso, si el buffer es pequeño, MPI_Send se comporta como MPI_BSend (envio buffered), y si el buffer es grande, se comporta como MPI_Ssend (envio sincronico). Por eso, al aumentar el tamaño del buffer, el programa se cuelga al ser sincronico, ya que el proceso 0 envia el temp y luego el mass, mientras que el proceso 1 espera recibir el mass y luego el temp.

  MPI_Finalize();
}
