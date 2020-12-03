#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---

    // TODO: MPI init

    // TODO: use MPI_Reduce
    long long circle = 0, temp[20];
    unsigned int seed;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    seed = (unsigned int)world_rank;
    for(int i = 0; i < tosses/world_size; i++)
    {
	double x = (double) rand_r(&seed)/RAND_MAX;
	double y = (double) rand_r(&seed)/RAND_MAX;
	if(x*x + y*y <= 1)
		circle ++;
    }
    MPI_Gather(&circle, 1, MPI_LONG_LONG, &temp[world_rank], 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    if (world_rank == 0)
    {
	 for(int i = 1; i<world_size; i++)
		 circle += temp[i];
        // TODO: PI result
	pi_result = (double) 4 * circle / tosses;

        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
