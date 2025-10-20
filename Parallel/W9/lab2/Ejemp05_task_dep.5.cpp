#include <iostream>
#include <omp.h>
#include <unistd.h>

// Elabore el DAG correspondiente al código

int main() {
  int a, b, c;

  #pragma omp parallel
  {
    #pragma omp master //* Genera las tasks y espera a que terminen
    {
      #pragma omp task depend(out : a)
      {
        a = 1;
        sleep(1);
        printf("Task 1, a: %d\n", a);
      }

      //* La task 2 consiste en la task 2 y 5, entonces 2 acabará hasta que 5 acabe
      #pragma omp task depend(out : b)
      {
        b = 2;
        sleep(1);
        printf("Task 2, b: %d\n", b);

        //* La dependencia se da primero por tareas del mismo nivel
        //* Los de niveles inferiores se ejecutan después, pero no antes 
        #pragma omp task depend(in : b) depend(out : c)
        {
          c = b + 3;
          sleep(3);
          printf("Task 5, c: %d\n", c);
        }
      }

      #pragma omp task depend(in : a, b) depend(out : c)
      {
        c = a + b + c;
        sleep(1);
        printf("Task 3, c: %d\n", c);
      }

      #pragma omp task depend(in : c)
      {
        sleep(1);
        printf("Task 4, c: %d\n", c);
      }
    }
  }

  return 0;
}
