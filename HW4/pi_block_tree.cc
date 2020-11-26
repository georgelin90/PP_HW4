#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---
    long long int circle = 0, temp = 0;
    // TODO: MPI init
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    srand(time(NULL));
    for(int i = 0; i < tosses/world_size; i++)
        {
            double x = (double) rand()/RAND_MAX;
            double y = (double) rand()/RAND_MAX;
            if(x*x + y*y <= 1)
                        circle ++;
        }

    // TODO: binary tree redunction
    for(int i = 1; i <= log(world_size); i *= 2)
    {
	    if(world_rank % i == 1)
		    MPI_Send(&circle, 1, MPI_LONG_LONG, world_rank - i/2, 0, MPI_COMM_WORLD);
	    else{
		    MPI_Recv(&temp, 1, MPI_LONG_LONG, world_rank + i/2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    circle += temp;
	    }
    }

    if (world_rank == 0)
    {
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
