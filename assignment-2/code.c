#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    int myrank;
    double stime, etime, proc_time1, proc_time2, max_time1, max_time2;
    
    MPI_Init(&argc, &argv);

    assert(argc == 4);
    int px = atoi(argv[1]); // number of processes in x
    int py = atoi(argv[2]); // number of processes in y
    long long size = atoll(argv[3]); // total number of elements in a row/column of the matrix

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if(!myrank){
        printf("px = %d, py = %d, size = %lld\n", px, py, size);
    }

    // 1D row-wise domain decomposition

    long long m = size / py; // number of rows of matrix for each process
    double A1[m][size]; // sub-domain of the process

    srand(time(NULL));

    // initializing the matrix
    for(int i = 0; i < m; i++){
        for(int j = 0; j < size; j++){
            A1[i][j] = rand();
        }
    }

    long long sendcount = myrank * m;
    long long recvcount = (myrank + 1) * m;
    double buf1[recvcount]; // receive buffer

    MPI_Request send_request1, recv_request1;
    MPI_Status send_status1, recv_status1;

    stime = MPI_Wtime();
    
    // updating the lower triangular matrix
    for(int t = 0; t < 20; t++){
        // sending elements of the first row to the above process
        if(myrank > 0){
            MPI_Isend(A1[0], sendcount, MPI_DOUBLE, myrank - 1, myrank - 1, MPI_COMM_WORLD, &send_request1);
        }

        // receiving elements of the first row of the below process
        if(myrank < py - 1){
            MPI_Irecv(buf1, recvcount, MPI_DOUBLE, myrank + 1, myrank, MPI_COMM_WORLD, &recv_request1);
        }

        if(myrank > 0){
            MPI_Wait(&send_request1, &send_status1);
        }

        // updating all rows except last row
        for(long long i = 0; i < m - 1; i++){
            // updating only the elements in the lower triangular matrix
            for(long long j = 0; j <= myrank * m + i; j++){
                A1[i][j] -= A1[i + 1][j];
            }
        }

        if(myrank < py - 1){
            MPI_Wait(&recv_request1, &recv_status1);
        }

        // updating the last row
        if(myrank < py - 1){
            for(long long j = 0; j < recvcount; j++){
                A1[m - 1][j] -= buf1[j];
            }
        }
    }

    etime = MPI_Wtime();
    proc_time1 = etime - stime;

    MPI_Reduce(&proc_time1, &max_time1, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if(!myrank){
        printf("1D row-wise domain decomposition time = %lf\n", max_time1);
    }

    // 2D domain decomposition

    m = size / py; // number of rows of matrix for each process
    long long n = size / px; // number of columns of matrix for each process
    double A2[m][n]; // sub-domain of the process

    // initializing the matrix
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            A2[i][j] = rand();
        }
    }

    double buf2[n]; // receive buffer

    MPI_Request send_request2, recv_request2;
    MPI_Status send_status2, recv_status2;
    
    int proc_row = myrank / px; // row of the process in the virtual topology
    int proc_col = myrank % px; // column of the process in the virtual topology

    stime = MPI_Wtime();

    // updating the lower triangular matrix
    for(int t = 0; t < 20; t++){
        // sending elements of the first row to the above process
        if(proc_row > 0){
            MPI_Isend(A2[0], n, MPI_DOUBLE, myrank - px, myrank - px, MPI_COMM_WORLD, &send_request2);
        }

        // receiving elements of the first row of the below process
        if(proc_row < py - 1){
            MPI_Irecv(buf2, n, MPI_DOUBLE, myrank + px, myrank, MPI_COMM_WORLD, &recv_request2);
        }

        if(proc_row > 0){
            MPI_Wait(&send_request2, &send_status2);
        }

        // updating all rows except last row
        for(long long i = 0; i < m - 1; i++){
            for(long long j = 0; j < n; j++){
                long long elem_row = proc_row * m + i; // row of the element in the complete matrix
                long long elem_col = proc_col * n + j; // column of the element in the complete matrix

                // updating the element if it is in the lower triangular matrix
                if(elem_row >= elem_col){
                    A2[i][j] -= A2[i + 1][j];
                }
            }
        }

        if(proc_row < py - 1){
            MPI_Wait(&recv_request2, &recv_status2);
        }

        // updating the last row
        if(proc_row < py - 1){
            for(long long j = 0; j < n; j++){
                long long elem_row = proc_row * m + (m - 1); // row of the element in the complete matrix
                long long elem_col = proc_col * n + j; // column of the element in the complete matrix

                // updating the element if it is in the lower triangular matrix
                if(elem_row >= elem_col){
                    A2[m - 1][j] -= buf2[j];
                }
            }
        }
    }

    etime = MPI_Wtime();
    proc_time2 = etime - stime;

    MPI_Reduce(&proc_time2, &max_time2, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if(!myrank){
        printf("2D domain decomposition time = %lf\n", max_time2);
    }

    MPI_Finalize();
    return 0;
}
