#define _POSIX_C_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>
#include <math.h>

const long int n = 10000000000;

double wtime(){
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double getrand(unsigned int *seed){
	return (double)rand_r(seed) / RAND_MAX;
}

double func(double x){
	return exp(x);	
}

int main(){
	double t0 = wtime();
	double s = 0;
	#pragma omp parallel reduction(+:s)
	{
		double s_loc = 0;
		unsigned int seed = omp_get_thread_num();
		#pragma omp for
		for(long int i = 0; i < n; i++){
			double x = getrand(&seed);
			s_loc += func(x);
		}
		
	s = s_loc;
	}
	double res = s / n;
	printf("%f\n", wtime() - t0);
	printf("Result: %.12f, n %ld\n", res, n);
	return 0;
}

// g++ -o mc openmp-mc.cpp -fopenmp
// ./mc
// export OMP_NUM_THREADS=4
