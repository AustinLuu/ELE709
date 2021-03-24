#include <stdio.h>
#include <math.h>
#include <sys/mman.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>

#define NTHREADS 3

#define SIZE 1000000

void *func(void *arg)
{
   int i, j, rcode;
   double *a;

   for (j = 0; ; j++) {
    a = (double *)calloc(SIZE, sizeof(double));
   	for (i = 0; i < SIZE; i++) {
      	a[i] = sqrt((double) i);
   	}
   	free(a);
   }

}

int main(int argc, char *argv[])
{
	pthread_t loads[NTHREADS];
	int no_of_threads;
	int i;

	no_of_threads = NTHREADS;

	printf("Created %d threads to simulate load on the system.\n", no_of_threads);

	for (i = 0; i < no_of_threads; i++) {
		if (pthread_create(&loads[i], NULL, &func, &i) != 0) {
			printf("Error creating thread %d\n", i);
			exit(-1);
		}
		else {
			printf("Created Thread %d\n", i);
		}
	}

	printf("\nRunning ...\n");
	printf("Use Ctrl-C to stop.\n");

	pthread_exit(NULL);
}
