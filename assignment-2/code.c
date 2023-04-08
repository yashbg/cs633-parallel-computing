#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    int myrank;
    double stime, etime, proc_time, max_time;
    
    MPI_Init(&argc, &argv);

    assert(argc == 4);
    int px = atoi(argv[1]);
    int py = atoi(argv[2]);
    long long size = atoll(argv[3]);

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if(!myrank){
        printf("px = %d, py = %d, size = %lld\n", px, py, size);
    }

    // 1D row-wise domain decomposition

    long long m = size / py; // number of rows of matrix for each process
    double A1[m][size];

    srand(time(NULL));

    // initializing the matrix
    for(int i = 0; i < m; i++){
        for(int j = 0; j < size; j++){
            A1[i][j] = rand();
        }
    }

    long long sendcount = myrank * m;
    long long recvcount = (myrank + 1) * m;
    double buf[recvcount];

    MPI_Request send_request, recv_request;
    MPI_Status send_status, recv_status;

    stime = MPI_Wtime();
    
    // updating the lower triangular matrix
    for(int t = 0; t < 20; t++){

        // sending elements of the first row to the previous process
        if(myrank > 0){
            MPI_Isend(A1[0], sendcount, MPI_DOUBLE, myrank - 1, myrank - 1, MPI_COMM_WORLD, &send_request);
        }

        // receiving elements of the first row of the next process
        if(myrank < py - 1){
            MPI_Irecv(buf, recvcount, MPI_DOUBLE, myrank + 1, myrank, MPI_COMM_WORLD, &recv_request);
        }

        MPI_Wait(&send_request, &send_status);

        // updating all rows except last row
        for(long long i = 0; i < m - 1; i++){
            // updating only the elements in the lower triangular matrix
            for(long long j = 0; j <= myrank * m + i; j++){
                A1[i][j] -= A1[i + 1][j];
            }
        }

        MPI_Wait(&recv_request, &recv_status);

        // updating the last row
        if(myrank < py - 1){
            for(long long j = 0; j < recvcount; j++){
                A1[m - 1][j] -= buf[j];
            }
        }
    }

    etime = MPI_Wtime();
    proc_time = etime - stime;

    MPI_Reduce(&proc_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if(!myrank){
        printf("1D row-wise domain decomposition time = %lf\n", max_time);
    }

    MPI_Finalize();
    return 0;
}
