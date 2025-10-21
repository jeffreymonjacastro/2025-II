#include <omp.h>
#include <iostream>
#include <vector>

int main() {
    const int N = 8;
    std::vector<int> A(N, 1), B(N);

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < N; ++i) {
                #pragma omp task firstprivate(i)
                B[i] = A[i] * A[i];
            }

            #pragma omp taskwait
            std::cout << "Todos los bloques procesados.\n";
        }
    }
}

