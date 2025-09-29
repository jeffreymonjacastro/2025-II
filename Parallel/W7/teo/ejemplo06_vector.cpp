/* Envia una columna de una matriz en rank 0
   a una fila de una matriz en rank 1
 * Input: ninguno
 * Output: la fila en rank 1
 * Nota:  debe ejecutarse en 2 procesos
 */

#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
    int p;
    int my_rank;
    int i, j,col=5;
    float A[10][10];
    MPI_Status status;

// 1. Declare tipo MPI

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

// 2. Defina tipo vector (columna 1)
//    MPI_Type_vector . . .
//    MPI_Type_commit . . . 

    if (my_rank == 0) {
        for (i = 0; i < 10; i++)
            for (j = 0; j < 10; j++)
                A[i][j] = (float) i;
// 3. Envie la columna al proceso 1
//        MPI_Send . . .
    } else { /* my_rank = 1 */
        for (i = 0; i < 10; i++)
            for (j = 0; j < 10; j++)
                A[i][j] = 0.0;
// 4. Reciba la columa en la primera fila en proceso 1
    //    MPI_Recv . . . 
        for (j = 0; j < 10; j++)
            printf("%3.1f ", A[0][j]);
        printf("\n");
    }
// 5. Libere memoria
    

MPI_Finalize();
}  /* main */
