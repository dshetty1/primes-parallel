//Name: Dhruv Shetty

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[]) {

    int comm_sz, my_rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Timing variabiles
    double local_start, local_finish, local_elapsed, elapsed;

    int number = atoi(argv[1]);
    int num_processes = comm_sz;
    
    // Dividing up the load per process. In this case, it's the number of primes allocated to each one.
    // Want this as evenly distributed as possible.
    // ceil((N+1)/P)
    int num_per_process = ((number+1) / num_processes) + (((number+1) % num_processes) != 0);

    int j;

    int * primes = malloc((number+num_processes) * sizeof(int));
    int local_fill = (my_rank) * num_per_process;
    int * local_primes = malloc(num_per_process * sizeof(int));

    MPI_Barrier(MPI_COMM_WORLD);

    // Start clock
    local_start = MPI_Wtime();

    // Creates each local array without multiples of 2
    for(int i = 0; i < num_per_process; i++){
        if(local_fill % 2 == 0)
            local_primes[i] = 0;
        else
            local_primes[i] = local_fill;
        local_fill++;
    }

    // Prime algorithm
    for(j=0; j<num_per_process; j++){
        for(int div = 3; div <= ((number+1)/2); div++){
            if(local_primes[j] == 0 || local_primes[j] <= div)
                continue;
            if ((local_primes[j] % div) == 0)
                local_primes[j] = 0;
        }
    }

    // Collective call (creates barrier) to send all local prime numbers to a master prime array controlled by process 0
    MPI_Gather(local_primes, num_per_process, MPI_INT, primes, num_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    // Stop clock and record time
    local_finish = MPI_Wtime();
    local_elapsed = local_finish-local_start;
    MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if(my_rank == 0)
        printf("Elapsed time = %f\n", elapsed);

    // Writes primes to output file
    if(my_rank==0){
        int count = 0;

        primes[2]=2;

        char filename[50];
        strcpy(filename, strcat(argv[1], ".txt"));

        FILE *output;
        output  = fopen(filename, "w");
        for (j = 2; j < number; j++) {
            if(primes[j] != 0){
                fprintf(output, "%d ", primes[j]);
                count++;
            }
        }
        fclose(output);
        printf("count: %d\n", count);
    }
       free(primes);
       free(local_primes);

    MPI_Finalize();

    return 0;
}
