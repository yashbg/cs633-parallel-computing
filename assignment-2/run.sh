mpicc ./code.c -o code
rm -f times.txt

for i in $(seq 1 5)
do 
	mpirun -np 4 -hosts csews7:4 ./code 2 2 4096
done

for i in $(seq 1 5)
do 
	mpirun -np 8 -hosts csews7:4,csews24:4 ./code 2 4 4096
done

for i in $(seq 1 5)
do 
	mpirun -np 4 -hosts csews7:4 ./code 2 2 32768
done

for i in $(seq 1 5)
do 
	mpirun -np 8 -hosts csews7:4,csews24:4 ./code 2 4 32768
done
