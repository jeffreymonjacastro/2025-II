#include <omp.h>
#include <iostream>

int main() {
    int N = 1<<10;
    double sum1 = 0, sum2 = 0;
    double start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task
            for (int i = 0; i < N/2; ++i)
                sum1 += i * 0.5;

            #pragma omp task
            for (int i = N/2; i < N; ++i)
                sum2 += i * 0.5;

            #pragma omp taskwait  // Esperar antes de combinar
            std::cout << "Suma total = " << sum1 + sum2 << "\n";
        }
    }

    double end = omp_get_wtime();
    std::cout << "Tiempo = " << end - start << " s\n";
}

