#include <omp.h>
#include <iostream>

int main() {
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task
            printf("Tarea A ejecutada por hilo %d\n",omp_get_thread_num() );

            #pragma omp task
            printf("Tarea B ejecutada por hilo %d\n",omp_get_thread_num() );

            #pragma omp task
            printf("Tarea C ejecutada por hilo %d\n",omp_get_thread_num() );
        }
    }
}

