#include <omp.h>
#include <iostream>
int main() {
    int tokA, tokB, tokC; 

    #pragma omp parallel
    #pragma omp single
    {
        #pragma omp task depend(out: tokA)
        { std::cout << "A\n"; }

        #pragma omp task depend(out: tokB)
        { std::cout << "B\n"; }

        #pragma omp task depend(out: tokC)
        { std::cout << "C\n"; }

        #pragma omp task depend(in: tokA, tokB, tokC)
        { std::cout << "Todo listo (A,B,C)\n"; }
    }
}

