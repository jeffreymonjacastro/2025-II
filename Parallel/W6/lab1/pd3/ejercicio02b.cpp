#include <iostream>
#include <mpi.h>
using namespace std;
int main(int argc, char *argv[]) {
  int N = 1 << 10;
  int my_rank, num_procs, rank;
  double *result_local = new double[N]{0};
  double *result_array = nullptr;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  // calculo en cada proceso
  for (int i = 0; i < N; i++)
    result_local[i] = 100.0 + 1.0 * my_rank;

  //  for(int i=0;i<N;i++) cout<<result_local[i]<<" ";
  if (my_rank == 0) {
    result_array = new double[N * num_procs]{0};
  }

  MPI_Gather(result_local, N, MPI_DOUBLE, result_array, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (my_rank == 0) { // imprime un elemento por proceso para control
    for (rank = 0; rank < num_procs; rank++)
      printf("Resultado desde el proceso %i es %f \n", rank, result_array[N * rank]);
  }

  MPI_Finalize();
}
