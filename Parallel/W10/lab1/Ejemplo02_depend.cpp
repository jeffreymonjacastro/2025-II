#include <omp.h>
#include <iostream>

int main(){
    const int N = 1000000;
    double sum1 = 0.0, sum2 = 0.0;

    #pragma omp parallel
    #pragma omp single
    {
        #pragma omp task depend(out: sum1) shared(sum1)
        for (int i = 0; i < N/2; ++i) 
            sum1 += i * 0.5;

        #pragma omp task depend(out: sum2) shared(sum2)
        for (int i = N/2; i < N; ++i) 
            sum2 += i * 0.5;

        // combina cuando ambas parciales estén listas
        #pragma omp task depend(in: sum1, sum2)
        { std::cout << "Suma total = " << (sum1 + sum2) << "\n"; }
    }
}

