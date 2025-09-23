//#include<mpi.h>
#include<iostream>
using namespace std;

int main(int argc, char*argv[]){
// declaracion de variables
int ierr, rank, size, dato=10;

// esta parte es estandar para los ejercicios de clase

// 1. ejecuta la impresion solo si MPI_Init resulto (use MPI_SUCCESS)

	printf("Mensaje del proceso %d, de un total de %d procesos \n", rank,size);
//cout<<"Mensaje del proceso "<<rank<<" de un total de " <<size<< " procesos \n";
// 2. modifica el valor de datos solo para que el proceso 2 tenga dato=20


}

