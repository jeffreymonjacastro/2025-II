#include <mpi.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

typedef long long ll;

int main(int argc, char** argv) {
    ll M = 1<<10,N = 1<<17;
    int rank, size;
    double t_comp = 0.0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc >= 3) { M = stoll(argv[1]); N = stoll(argv[2]); }
    if (M <= 0 || N <= 0) {
        if (rank == 0) {
            cerr<<"Error: M y N deben ser positivos.\n";
        }
        MPI_Finalize();
        return 1;
    }

    if (M % size != 0) {
        if (rank == 0) {
            cerr<<"Error: M debe ser divisible por el numero de procesos.\n";
        }
        MPI_Finalize();
        return 1;
    }

    // Calcular el número de filas por proceso
    ll rows_per_proc = M / size, local_M = rows_per_proc;
    
    vector<double> A_global, B_global, C_global(M, 0.0);
    vector<double> A_local(local_M * N), B_local(local_M * N);
    vector<double> C_local(local_M, 0.0);

    if (rank == 0) {
        A_global.resize(M * N);
        B_global.resize(M * N);
        for (ll j = 0; j < M; ++j) {
            for (ll i = 0; i < N; ++i) {
                A_global[j*N + i] = 1.0 + (i % 7) * 0.01 + j * 0.001;
                B_global[j*N + i] = 2.0 + (i % 5) * 0.02 + j * 0.002;
            }
        }
    }

    // Datos a todos los procesos
    MPI_Scatter(A_global.data(), local_M * N, MPI_DOUBLE, A_local.data(), local_M * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(B_global.data(), local_M * N, MPI_DOUBLE, B_local.data(), local_M * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    // Cálculo local
    for (ll j = 0; j < local_M; ++j) {
        const ll base = j * N;
        double acc = 0.0;
        for (ll i = 0; i < N; ++i) {
            acc += A_local[base + i] * B_local[base + i];
        }
        C_local[j] = acc;
    }

    double t1 = MPI_Wtime();
    double elapsed_time = (t1 - t0);
    double max_time;

    MPI_Reduce(&elapsed_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // Gather de los resultados
    MPI_Gather(C_local.data(), local_M, MPI_DOUBLE, C_global.data(), local_M, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout<<fixed<<setprecision(6);
        cout<<"M="<<M<<" N="<<N<<" tiempo de computo (MAX) = "<<max_time * 1000<<" ms\n";
    }

    MPI_Finalize();
    return 0;
}