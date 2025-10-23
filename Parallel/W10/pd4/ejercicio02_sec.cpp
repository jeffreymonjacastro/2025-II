#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <ctime>
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

    auto t0 = chrono::high_resolution_clock::now();

    for (uint64_t i = 0; i < N; ++i) {
        // Uniformes en (0,1)
        double u = static_cast<double>(rand()) / RAND_MAX;
        double v = static_cast<double>(rand()) / RAND_MAX;

        double x = std::min(u, v);
        double y = std::max(u, v);

        double s1 = x;
        double s2 = y - x;   // 1 resta
        double s3 = 1.0 - y; // 1 resta

        double smax = std::max({s1, s2, s3});
        if (smax < 0.5) {        // condición de triángulo
            ++tri_cnt;
            double q1 = s1 * s1; // 3 mults
            double q2 = s2 * s2;
            double q3 = s3 * s3;
            double S  = q1 + q2 + q3; // 2 sumas
            double qmax = std::max({q1, q2, q3});
            if (qmax + qmax > S) {    // obtuso
                ++obt_cnt;
            }
        }
    }

    auto t1 = chrono::high_resolution_clock::now();
    double secs = chrono::duration<double>(t1 - t0).count();

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
//    unsigned int seed = (argc > 2) ? static_cast<unsigned int>(strtoul(argv[2], nullptr, 10))
  //                                 : static_cast<unsigned int>(time(nullptr));
    unsigned int seed = time(nullptr);
    srand(seed); // inicializa RNG C

    auto R = run_mc_seq(N);

    cout.setf(ios::fixed);
    cout << setprecision(6);
    cout << "Trials          : " << N << "\n";
    cout << "Seed (srand)    : " << seed << "\n";
    cout << "P(triangulo)    : " << R.p_tri << "\n";
    cout << "P(obtuso)       : " << R.p_obt << "  (no cond.)\n";
    cout << "Tiempo (s)      : " << R.secs << "\n";
    cout << "Rendimiento     : " << R.gflops << " GFLOP/s\n";
    cout << "Nota: 8 FLOPs/muestra (geometria), RNG no incluido.\n";
}

