#include <iostream>
#include <mpi.h>

int main ( int argc, char *argv[] )
{
  int rank,N=3,M=6,stride=6;
  int p;

  MPI_Init ( &argc, &argv );
  MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
  MPI_Comm_size ( MPI_COMM_WORLD, &p );
  MPI_Status status;

double A[N][M];
/*
| 0 | 1 | 2 | 3  4 | 5 |
| 0 | 1 | 2 | 3  4 | 5 |
| 0 | 1 | 2 | 3  4 | 5 |
*/

// 1. Declare y defina tipo MPI (vector de un bloque de 2x3)
MPI_Datatype bloque_m; 
MPI_Type_vector(2,3,stride,MPI_DOUBLE,&bloque_m);
MPI_Type_commit(&bloque_m);

if (rank==0) {
printf("rank: %d \n",rank);
for (int i=0; i<N;i++) { 
for (int j=0; j<M;j++) {
	A[i][j]=j;
	printf("%f ",A[i][j]);}
printf("\n");
}
// 2. Envie bloque de rank 0 a 1
MPI_Send(&A[0][1],1,bloque_m,1,0,MPI_COMM_WORLD);
} 

else if (rank==1) {
double subdominio[N][M];
// 3. Recibe bloque de rank 0 
MPI_Recv(&subdominio,1,bloque_m,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

printf("rank: %d \n",rank);
for (int i=0; i<N;i++) { 
for (int j=0; j<M;j++) {
	printf("%f ",subdominio[i][j]);}
printf("\n");
}
}
// 4. Libere memoria
MPI_Type_free(&bloque_m);
}
