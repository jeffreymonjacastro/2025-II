#include <omp.h>
#include <iostream>

// (opcional) versión secuencial para granularidad
static inline int fib_seq(int n){
    if(n < 2) return n;
    return fib_seq(n-1) + fib_seq(n-2);
}

int fib(int n){
    if(n < 20) return fib_seq(n);  // umbral para evitar explosión de tareas
    int x = 0, y = 0, res = 0;

    #pragma omp taskgroup
    {
        // Produce x
        #pragma omp task shared(x) depend(out: x)
        x = fib(n - 1);

        // Produce y
        #pragma omp task shared(y) depend(out: y)
        y = fib(n - 2);

        // Suma cuando x e y estén listos
        #pragma omp task depend(in: x, y) shared(res, x, y)
        { res = x + y; }
    } // <- aquí el taskgroup espera a las 3 tareas; x e y siguen vivos

    return res;
}

int main(){
    int n = 10, result = 0;
    #pragma omp parallel
    #pragma omp single
    {
        result = fib(n);
        std::cout << "Fib(" << n << ") = " << result << "\n";
    }
}

