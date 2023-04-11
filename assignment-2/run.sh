mpicc ./code.c -o code

for i in $(seq 1 5)
do 
	mpirun -np 4 ./code 2 2 4096
done

for i in $(seq 1 5)
do 
	mpirun -np 8 ./code 2 4 4096
done

for i in $(seq 1 5)
do 
	mpirun -np 4 ./code 2 2 32768
done

for i in $(seq 1 5)
do 
	mpirun -np 8 ./code 2 4 32768
done
