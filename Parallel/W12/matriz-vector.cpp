#include <iostream>
#define N 8
using namespace std;

int main(int argc,char *argv[]){

    int i, j;
    int A[N*N], local_A[N*N];
    int v[N], x[N], local_x[N];

        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++)
                A[i*N+j] = 1+2*i+j;
            v[i] = 1+3*i;
        }
// -------------------------------------------------------------------------
    clock_t start = clock();

    for (i = 0; i < N; i++) {
        x[i] = 0;
        for (j = 0; j < N; j++) {
            x[i] += A[i*N+j] * v[j];
        }
    }
 clock_t end = clock();
    double elapsed = double(end - start)/CLOCKS_PER_SEC;

// -------------------------------------------------------------------------

        printf("Resultado:\n");
        for (i = 0; i < N; i++) printf("%d ", x[i]);
    printf("\nTiempo: %.8f seconds.\n", elapsed);
}

