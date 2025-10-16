/*
 * @@name:	task_dep.3c
 * @@type:	C
 * @@compilable:	yes
 * @@linkable:	yes
 * @@expect:	success
 */
#include <omp.h>
#include <stdio.h>
int main() {
  int x;
	#pragma omp parallel
	#pragma omp single
	{
		#pragma omp task 
		x = 2;

		#pragma omp task shared(x) depend(in : x)
		printf("x+1= %d\n", x + 1);

		#pragma omp task shared(x) depend(in : x)
		printf("x+2= %d\n", x + 2);
  }

  return 0;
}
