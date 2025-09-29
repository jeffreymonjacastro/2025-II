/* envia una fila de la matriz A de 0 a 1
 * Input: ninguno
 * Output: fila recibida por proceso 1
 * Nota:  debe ejecutarse en 2 procesos
 */

#include <iostream>
#include <mpi.h>
#define N 6
#define M 6
#define f 1
int main(int argc, char* argv[]) {
    int i, j;
    int p;
    int my_rank;
    float A[N][M]={0};
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0) {
        for (i = 0; i < N; i++)
            for (j = 0; j < M; j++)
                A[i][j] = (float) i;
// 1. Rank 0 envíauna fila de matriz A a proceso 1
      MPI_Send(A[f],M,MPI_FLOAT,1,0,MPI_COMM_WORLD); 
    } else { /* my_rank = 1 */
// 2. Rank 1 recibe e imprime su matriz con la fila enviada
      MPI_Recv(A[f],M,MPI_FLOAT,0,0,MPI_COMM_WORLD,&status); 
        for (i = 0; i < N; i++){
		for (j = 0; j < M; j++)
            		printf("%3.1f ", A[i][j]);
        printf("\n");
    }}

    MPI_Finalize();
} 
