#include <mpi.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>


using namespace std;

typedef long long ll;

int main (int argc, char **argv) {
    ll N = 1LL << 20;
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        N = (N / size) * size;
        if (rank == 0) {
            cout << "N ajustado a " << N << " para ser divisible por " << size << " procesos.\n";
        }
    }

    ll local_N = N / size;
    vector<double> global_in(N), global_out(N, 0.0);
    vector<double> local_in(local_N + 2), local_out(local_N);
    
    if (rank == 0) {
        for (ll i = 0; i < N; ++i) {
            global_in[i] = static_cast<double>(i % 101);
        }
    }
    
    MPI_Scatter(global_in.data(), local_N, MPI_DOUBLE, &local_in[1], local_N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double t_update = 0.0;
    
    auto t0 = chrono::high_resolution_clock::now();

    MPI_Request reqs[4];
    MPI_Status stats[4];
    
    if (rank > 0) {
        MPI_Isend(&local_in[1], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &reqs[0]);
        MPI_Irecv(&local_in[0], 1, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, &reqs[1]);
    }
    
    if (rank < size - 1) {
        MPI_Isend(&local_in[local_N], 1, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, &reqs[2]);
        MPI_Irecv(&local_in[local_N + 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &reqs[3]);
    }

    for (ll i = 2; i <= local_N - 1; ++i) {
        local_out[i] = (local_in[i-1] + local_in[i] + local_in[i+1]) / 3.0;
    }

    if (rank > 0 && rank < size - 1) {
        MPI_Waitall(4, reqs, stats);
    } else if (rank == 0) {
        MPI_Waitall(2, &reqs[2], &stats[2]);
    } else {
        MPI_Waitall(2, reqs, stats);
    }
    
    if (rank == 0) {
        local_out[0] = (local_in[1] + local_in[1] + local_in[2]) / 3.0;
    } else {
        local_out[0] = (local_in[0] + local_in[1] + local_in[2]) / 3.0;
    }
    
    if (rank == size - 1) {
        local_out[local_N - 1] = (local_in[local_N - 1] + local_in[local_N] + local_in[local_N]) / 3.0;
    } else {
        local_out[local_N - 1] = (local_in[local_N - 1] + local_in[local_N] + local_in[local_N + 1]) / 3.0;
    }

    MPI_Gather(local_out.data(), local_N, MPI_DOUBLE, global_out.data(), local_N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    auto t1 = chrono::high_resolution_clock::now();
    t_update = chrono::duration<double>(t1 - t0).count();
    
    if (rank == 0) {
        cout << "N=" << N << " t_update (ms): " << t_update * 1000 << "\n";
    }

    MPI_Finalize();
    return 0;
}
