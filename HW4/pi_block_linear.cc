#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    long long circle = 0, temp = 0;
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---

    // TODO: init MPI
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank > 0)
    {
	srand(time(NULL));
	for(int i = 0; i < tosses/world_size; i++)
	{
		double x = (double) rand()/RAND_MAX;
		double y = (double) rand()/RAND_MAX;
		if(x*x + y*y <= 1)
			circle ++;
        }
	MPI_Send(&circle, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);

    }
    else if (world_rank == 0)
    {
        // TODO: master
	srand(time(NULL));
        for(int i = 0; i < tosses/world_size; i++)
        {
                double x = (double) rand()/RAND_MAX;
                double y = (double) rand()/RAND_MAX;
                if(x*x + y*y <= 1)
                        circle ++;
        }
	for( int i = 1; i < world_size; i++)
	{
	MPI_Recv(&temp, 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	circle += temp;
	}
    }

    if (world_rank == 0)
    {
        // TODO: process PI result
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
