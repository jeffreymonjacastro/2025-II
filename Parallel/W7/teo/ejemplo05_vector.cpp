/* Envia una columna de una matriz del proceso 0 al 1
 * Input:  Ninguno
 * Output: proceso 1 recibe e imprime la columna
 * Nota: se debe ejecutar solo en 2 procesos
 */
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
    int p;
    int my_rank;
    float A[10][10];
    MPI_Status status;
    int i, j,col=6;
// 1. Declare el tipo MPI
 MPI_Datatype columna; 

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

// 2. Defina el tipo vector para enviar la columna col
    MPI_Type_vector(10,1,10,MPI_FLOAT,&columna);
    MPI_Type_commit(&columna);

// 3.. Envie la columna desde el rank o al 1
    if (my_rank == 0) {
        for (i = 0; i < 10; i++)
            for (j = 0; j < 10; j++)
                A[i][j] = (float) j;

    MPI_Send(&A[0][col],1,columna,1,0,MPI_COMM_WORLD);
    } else { /* my_rank = 1 */
    MPI_Recv(&A[0][0],1,columna,0,0,MPI_COMM_WORLD,&status);
        for (i = 0; i < 10; i++)
            	printf("%3.1f ", A[i][0]);
        printf("\n");
    }
// 4. Libere memoria
MPI_Type_free(&columna);
  
  MPI_Finalize();
} 
