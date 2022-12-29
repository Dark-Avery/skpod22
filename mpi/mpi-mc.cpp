#define _POSIX_C_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <sys/time.h>
#include <math.h>


double getrand(unsigned int *seed){
	return (double)rand_r(seed) / RAND_MAX;
}

double func(double x){
	return exp(x);	
}

int main(int argc, char *argv[]){
    double s = 0, t0, res;
    int numprocs, myid;
    long long int n = 1000000000;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid); 
    if (myid == 0) {
        t0 = MPI_Wtime();
    }
        double s_loc = 0;
        unsigned int seed = myid;
        for (long long int i = myid; i < n; i += numprocs){
            double x = getrand(&seed);
            s_loc += func(x);
        }   
    MPI_Reduce(&s_loc, &s, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    // MPI_Allreduce(&s_loc, &s, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    if (myid == 0) {
        res = s / n;
        printf("%f\n", MPI_Wtime() - t0);
        printf("Result: %12f, n %lld\n", res, n);
    }
	MPI_Finalize();
	return 0;
}

// mpicxx -o mc mpi-mc.cpp
// mpiexec -np 4 ./mc
