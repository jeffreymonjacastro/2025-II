#include <omp.h>
#include <iostream>

int main() {
    int data = 1;      // dato a procesar
    int t12, t23, done; // tokens dummy (solo sincronizan por dirección)

    #pragma omp parallel
    #pragma omp single
    {
        // Etapa 1 produce t12
        #pragma omp task shared(data) depend(out: t12)
        { data *= 2; std::cout << "Etapa 1: " << data << "\n"; }

        // Etapa 2 espera t12 y produce t23
        #pragma omp task shared(data) depend(in: t12) depend(out: t23)
        { data += 3; std::cout << "Etapa 2: " << data << "\n"; }

        // Etapa 3 espera t23 y produce done
        #pragma omp task shared(data) depend(in: t23) depend(out: done)
        { data *= data; std::cout << "Etapa 3: " << data << "\n"; }

        // Join final: espera 'done' (sin taskwait)
        #pragma omp task depend(in: done)
        { std::cout << "Pipeline terminado. data = " << data << "\n"; }
    }
}

/*
#include <omp.h>
#include <iostream>

int main(){
    int data = 1;

    #pragma omp parallel
    #pragma omp single
    {
        // Cada etapa lee y escribe 'data' -> usar inout serializa el flujo
        #pragma omp task depend(inout: data)
        { data *= 2; std::cout << "Etapa 1: " << data << "\n"; }

        #pragma omp task depend(inout: data)
        { data += 3; std::cout << "Etapa 2: " << data << "\n"; }

        #pragma omp task depend(inout: data)
        { data *= data; std::cout << "Etapa 3: " << data << "\n"; }
    }
}
*/
