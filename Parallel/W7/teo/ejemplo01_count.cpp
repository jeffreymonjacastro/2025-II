/* count.c -- envia un subvector del proceso 0 al proceso 1
 * Input: ninguno
 * Output: vector recibido por el proceso 1
 * Nota: Debe ejecutarse en 2 procesos
 */

#include <iostream>
#include <mpi.h>
using namespace std;
int main(int argc, char* argv[]) {
    int p;
    int my_rank;
    int i;
    float vector[10];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /*  Initialice vector en 0 y envio a 1*/
    if (my_rank == 0) {
        for (i = 0; i < 10; i++)
		if(i<5)	vector[i] = 0.0;
            	else 		vector[i] = 5.0;
	cout<<"vector origen: "; 
        for (i = 0; i < 10; i++)
            printf("%3.1f ",vector[i]);
        printf("\n");
// 1. Envie la segunda mitad del vector al proceso 1
        MPI_Send(vector+5,5,MPI_FLOAT,1,0,MPI_COMM_WORLD);  

    } else { /* my_rank == 1 */
// 2. Proceso 1 recibe la segunda mitad del vector desde 0
        MPI_Recv(vector,5,MPI_FLOAT,0,0,MPI_COMM_WORLD,&status); 
	cout<<"vector destino: "; 
        for (i = 0; i < 10; i++)
            printf("%3.1f ",vector[i]);
        printf("\n");
    }

    MPI_Finalize();
}
