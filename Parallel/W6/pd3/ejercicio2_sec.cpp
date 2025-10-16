#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>

int main(int argc, char** argv) {
    long long M = 1<<10;      // número de vectores
    long long N = 1<<17;    // tamaño de cada vector
    double t_comp  = 0.0;
    if (argc >= 3) { M = std::stoll(argv[1]); N = std::stoll(argv[2]); }
    if (M <= 0 || N <= 0) {
        std::cerr << "Error: M y N deben ser positivos.\n";
        return 1;
    }

    // Arreglos planos: fila j ocupa posiciones [j*N .. j*N+N-1]
    std::vector<double> A(M * N), B(M * N), C(M, 0.0);

    // Inicialización de ejemplo (cámbiala por tu lectura real si corresponde)
    for (long long j = 0; j < M; ++j)
        for (long long i = 0; i < N; ++i) {
            A[j*N + i] = 1.0 + (i % 7) * 0.01 + j * 0.001;
            B[j*N + i] = 2.0 + (i % 5) * 0.02 + j * 0.002;
        }

//    auto t0 = std::chrono::high_resolution_clock::now();
    clock_t t0 = clock();
    // Cálculo: C[j] = sum_i A[j,i] * B[j,i]
    for (long long j = 0; j < M; ++j) {
        const long long base = j * N;
        double acc = 0.0;
        for (long long i = 0; i < N; ++i)
            acc += A[base + i] * B[base + i];
        C[j] = acc;
    }

    clock_t t1 = clock();
    t_comp = 1000.0*(t1 - t0) / CLOCKS_PER_SEC;
   // auto t1 = std::chrono::high_resolution_clock::now();
   // double secs = std::chrono::duration<double>(t1 - t0).count();

    // Reporte
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "M=" << M << " N=" << N << "  tiempo = " << t_comp << " ms\n";
    return 0;
}

