#include <omp.h>
#include <iostream>

#include <omp.h>
#include <iostream>

int main() {
    int data = 1;

    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task
            { data *= 2; std::cout << "Etapa 1: " << data << "\n"; }

            #pragma omp taskwait

            #pragma omp task
            { data += 3; std::cout << "Etapa 2: " << data << "\n"; }

            #pragma omp taskwait

            #pragma omp task
            { data *= data; std::cout << "Etapa 3: " << data << "\n"; }
        }
    }
}
