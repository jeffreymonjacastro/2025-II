#include <omp.h>
#include <iostream>

int fib(int n) {
    if (n < 2) return n;
    int x, y;
    #pragma omp task shared(x)
    x = fib(n - 1);
    #pragma omp task shared(y)
    y = fib(n - 2);
    #pragma omp taskwait
    return x + y;
}

int main() {
    int n = 10, result;
    #pragma omp parallel
    {
        #pragma omp single
        result = fib(n);
    }
    std::cout << "Fib(" << n << ") = " << result << "\n";
}

