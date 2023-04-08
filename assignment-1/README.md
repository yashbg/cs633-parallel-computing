# CS633: Parallel Computing
## Assignment 1 
## Name : Akshat Sharma (190090), Yash Gupta (190997)

## Submission contents:
1. RUN_ME.sh : executes the files (run.py) and (make_plots.py)
1. code.c : Contains the code to execute send-receive between two processes. Transmission time is computed using the MPI_Reduce function and saved to the (times.txt) file.
2. hosts.txt : contains the hostnames which are passed to the (mpirun) function.
3. run.py : This python file creates an mpi process for each configuration, i.e. between each host pair and for each data size. The time taken for each configuration is written to the (times.txt) file in a newline.
4. make_plots.py : Reads the (times.txt) file and computes the transmission time averaged over multiple runs and pairs of nodes. Plots are written to directory (plots/)

## Produced files:
1. plots/ : plots from the experiments.
4. times.txt : Transmission times are written to this file.
2. code : The ELF binary produced upon compiling (code.c).
3. logs.txt : Debug info for (run.py)
