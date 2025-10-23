#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#ifdef _OPENMP
    #include <omp.h>
#endif

using namespace std;

struct Result {
    double p_tri;
    double p_obt;
    double secs;
    double gflops;
};

Result run_mc_seq(uint64_t N) {
    const double FLOPS_PER_TRIAL = 8.0; // FLOPs geométricos aprox. por muestra

    uint64_t tri_cnt = 0;
    uint64_t obt_cnt = 0;
    double secs = 0.0;

    #ifdef _OPENMP
        double t0 = omp_get_wtime();
    #endif

    #pragma omp parallel
    {
        uint64_t tri_cnt_local = 0;
        uint64_t obt_cnt_local = 0;

        #pragma omp for
        for (uint64_t i = 0; i < N; ++i) {
            // Usando rand_r para thread-safety
            unsigned int seed = static_cast<unsigned int>(time(nullptr)) + static_cast<unsigned int>(i * 1103515245);

            double u = static_cast<double>(rand_r(&seed)) / RAND_MAX;
            double v = static_cast<double>(rand_r(&seed)) / RAND_MAX;

            double x = std::min(u, v);
            double y = std::max(u, v);

            double s1 = x;
            double s2 = y - x;   // 1 resta
            double s3 = 1.0 - y; // 1 resta

            double smax = std::max({s1, s2, s3});

            if (smax < 0.5) {        // condición de triángulo
                ++tri_cnt_local;

                double q1 = s1 * s1; // 3 mults
                double q2 = s2 * s2;
                double q3 = s3 * s3;
                double S  = q1 + q2 + q3; // 2 sumas
                double qmax = std::max({q1, q2, q3});

                if (qmax + qmax > S) {    // obtuso
                    ++obt_cnt_local;
                }
            }
        }

        #pragma omp critical
        {
            tri_cnt += tri_cnt_local;
            obt_cnt += obt_cnt_local;
        }
    }
    #ifdef _OPENMP
        double t1 = omp_get_wtime();
        secs = t1 - t0;
    #endif

    double p_tri = static_cast<double>(tri_cnt) / static_cast<double>(N);
    double p_obt = static_cast<double>(obt_cnt) / static_cast<double>(N);
    double total_flops = FLOPS_PER_TRIAL * static_cast<double>(N);
    double gflops = (total_flops / secs) / 1e9;

    return {p_tri, p_obt, secs, gflops};
}

int main(int argc, char** argv) {
    // Uso:
    //   ./a.out [N]
    // N por defecto = 1e8, seed por defecto = time(nullptr)
    uint64_t N = (argc > 1) ? strtoull(argv[1], nullptr, 10) : 100000000ULL;
    unsigned int seed = time(nullptr);
    srand(seed); // inicializa RNG C

    int num_threads = 1;
    #ifdef _OPENMP
        #pragma omp parallel
        {
            #pragma omp single
            num_threads = omp_get_num_threads();
        }
    #endif
    cout << "Usando OpenMP con " << num_threads << " threads\n";

    const int NUM_RUNS = 5;
    double sum_p_tri = 0.0;
    double sum_p_obt = 0.0;
    double sum_secs = 0.0;
    double sum_gflops = 0.0;

    cout.setf(ios::fixed);
    cout << setprecision(6);

    for (int run = 1; run <= NUM_RUNS; ++run) {
        auto R = run_mc_seq(N);
        
        cout << "Ejecución " << run << ":\n";
        cout << "  P(triangulo) : " << R.p_tri << "\n";
        cout << "  P(obtuso)    : " << R.p_obt << "\n";
        cout << "  Tiempo (s)   : " << R.secs << "\n";
        cout << "  GFLOP/s      : " << R.gflops << "\n\n";

        sum_p_tri += R.p_tri;
        sum_p_obt += R.p_obt;
        sum_secs += R.secs;
        sum_gflops += R.gflops;
    }

    // Calcular promedios
    double avg_p_tri = sum_p_tri / NUM_RUNS;
    double avg_p_obt = sum_p_obt / NUM_RUNS;
    double avg_secs = sum_secs / NUM_RUNS;
    double avg_gflops = sum_gflops / NUM_RUNS;

    cout << "========================================\n";
    cout << "PROMEDIOS DE " << NUM_RUNS << " EJECUCIONES EN PARALELO:\n";
    cout << "========================================\n";
    cout << "Trials              : " << N << "\n";
    cout << "Seed (srand)        : " << seed << "\n";
    cout << "P(triangulo) prom.  : " << avg_p_tri << "\n";
    cout << "P(obtuso) prom.     : " << avg_p_obt << "  (no cond.)\n";
    cout << "Tiempo (s) prom.    : " << avg_secs << "\n";
    cout << "Rendimiento prom.   : " << avg_gflops << " GFLOP/s\n";
    cout << "Nota: 8 FLOPs/muestra (geometria), RNG no incluido.\n";
}

