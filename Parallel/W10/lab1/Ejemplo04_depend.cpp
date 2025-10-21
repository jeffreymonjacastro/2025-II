#include <omp.h>
#include <vector>
#include <iostream>

int main(){
    const int N = 8;
    std::vector<int> A(N,1), B(N);
    int tok[8]; // ← tokens “estilo C”

    #pragma omp parallel
    #pragma omp single
    {
        for (int i = 0; i < N; ++i) {
            #pragma omp task firstprivate(i) shared(A,B,tok) depend(out: tok[i])
            B[i] = A[i] * A[i];
        }

        // join: depende de todos los tokens
        #pragma omp task shared(tok) \
                        depend(in: tok[0], tok[1], tok[2], tok[3], tok[4], tok[5], tok[6], tok[7])
        std::cout << "Todos los bloques listos.\n";
    }
}

