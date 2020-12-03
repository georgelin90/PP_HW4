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
    long long circle = 0, temp[20];
    // TODO: MPI init
    unsigned int seed;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank > 0)
    {
	seed = (unsigned int) world_rank;
        // todo: mpi workers
	for( int i = 0; i<tosses/world_size; i++)
	{
		double x = (double) rand_r(&seed) / RAND_MAX;
		double y = (double) rand_r(&seed) / RAND_MAX;
		if( x*x + y*y <= 1)
			circle ++;
	}
	MPI_Send(&circle, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }
    else if (world_rank == 0)
    {
        MPI_Request requests[20];
	for(int i = 1; i<world_size; i++)
		MPI_Irecv(&temp[i], 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD, &requests[i]);
        // todo: non-blocking mpi communication.	
	seed = (unsigned int) world_size;
	for( int i = 0; i<tosses/world_size; i++)
	{
		double x = (double) rand_r(&seed) / RAND_MAX;
		double y = (double) rand_r(&seed) / RAND_MAX;
		if( x*x + y*y <= 1)
			circle ++;
	}
        MPI_Waitall(world_size-1, &requests[1], MPI_STATUS_IGNORE);
    }

    if (world_rank == 0)
    {
	
        // todo: pi result
	for(int i = 1; i<world_size ; i++)
		circle += temp[i];
	pi_result = (double) 4 * circle / tosses;
        // --- don't touch ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("mpi running time: %lf seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
