#include <chrono>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  long long N = 1LL << 20; // 2^20 por defecto
  std ::vector<double> global_in(N), global_out(N, 0.0);

  // Inicializacion
  for (long long i = 0; i < N; ++i)
    global_in[i] = double(i % 101);

  double t_update = 0.0; // tiempo real de computo ( suavizado )

  // ===== Suavizado 1D =====
  auto t0 = std ::chrono ::high_resolution_clock ::now();

  // i = 0 ( borde izquierdo , clonado )
  global_out[0] = (global_in[0] + global_in[0] + global_in[1]) / 3.0;
  // internos

  for (long long i = 1; i <= N - 2; ++i) {
    global_out[i] = (global_in[i - 1] + global_in[i] + global_in[i + 1]) / 3.0;
  }

  // i = N -1 ( borde derecho , clonado )
  global_out[N - 1] = (global_in[N - 2] + global_in[N - 1] + global_in[N - 1]) / 3.0;

  auto t1 = std ::chrono ::high_resolution_clock ::now();

  t_update = std ::chrono ::duration<double>(t1 - t0).count();
  std ::cout << "N=" << N << " t_update (ms ): " << t_update << "\n";
  return 0;
}