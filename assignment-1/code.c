#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mpi.h"

int main(int argc, char *argv[]){
    int myrank; 
    MPI_Status status;
    double sTime, eTime, time, maxTime;
    FILE *fptr;

    MPI_Init(&argc, &argv);

    assert(argc == 2);  // number of bytes to be transferred comes from the input
    int count = atoi (argv[1]);
    int buf[count];

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    // initialize data
    for(int i = 0; i < count; i++){
        buf[i] = myrank+i;
    }

    sTime = MPI_Wtime();
    if(!myrank){
        MPI_Send(buf, count, MPI_INT, 1, 1, MPI_COMM_WORLD);	   
    }
    else if(myrank == 1){
        MPI_Recv(buf, count, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);	   
    }
    eTime = MPI_Wtime();
    time = eTime - sTime;

    // obtain max time
    MPI_Reduce(&time, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if(!myrank){
        fptr = fopen("times.txt", "a");
        fprintf(fptr, "%lf\n", maxTime);
        fclose(fptr);
    }

    MPI_Finalize();
    return 0;
}
