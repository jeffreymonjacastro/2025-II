#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

void run_ping_pong_test(int rank, int size, long message_size, const string& mode, bool is_ssend) {
    if (size != 2) {
        if (rank == 0) {
            cout << "Este programa requiere exactamente dos procesos para funcionar." << endl;
        }
        return;
    }

    const int N_warm = 10;
    const int N = 100;
    double ini, fin;
    vector<char> send_buffer(message_size);
    vector<char> recv_buffer(message_size);

    // Bucle para calentar (warm-up)
    for (int i = 0; i < N_warm; ++i) {
        if (rank == 0) {
            if (is_ssend) {
                MPI_Ssend(send_buffer.data(), message_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(recv_buffer.data(), message_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Send(send_buffer.data(), message_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(recv_buffer.data(), message_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        } else {
            if (is_ssend) {
                MPI_Recv(recv_buffer.data(), message_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Ssend(send_buffer.data(), message_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            } else {
                MPI_Recv(recv_buffer.data(), message_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(send_buffer.data(), message_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD); // Sincronización antes de la medición
    ini = MPI_Wtime();

    // Bucle para la medicion
    for (int i = 0; i < N; ++i) {
        if (rank == 0) {
            if (is_ssend) {
                MPI_Ssend(send_buffer.data(), message_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(recv_buffer.data(), message_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Send(send_buffer.data(), message_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(recv_buffer.data(), message_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        } else {
            if (is_ssend) {
                MPI_Recv(recv_buffer.data(), message_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Ssend(send_buffer.data(), message_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            } else {
                MPI_Recv(recv_buffer.data(), message_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(send_buffer.data(), message_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            }
        }
    }

    fin = MPI_Wtime();
    
    if (rank == 0) {
        double total_time = fin - ini;
        double latency = total_time / (N * 2.0) * 1e6; // Latencia en microsegundos (tiempo por mensaje)
        double bandwidth = (2.0 * N * message_size) / (total_time * 1e6); // Ancho de banda en MB/s
        
        cout << mode << " | Tamanio: " << message_size << " bytes | Latencia: " << latency << " us | Ancho de Banda: " << bandwidth << " MB/s" << endl;
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Tamaños de mensaje en bytes
    vector<long> message_sizes = {8, 512, 32 * 1024, 2 * 1024 * 1024};

    for (long msg_size : message_sizes) {
        run_ping_pong_test(rank, size, msg_size, "MPI_Send", false);
        run_ping_pong_test(rank, size, msg_size, "MPI_Ssend", true);
    }
    
    MPI_Finalize();
    return 0;
}