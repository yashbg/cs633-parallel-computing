import subprocess

subprocess.run(['rm', '-f', 'times.txt'])   # clean the file containing runtimes
subprocess.run(['rm', '-f', 'logs.txt'])    # clean the file containing message logs
subprocess.run(['mpicc', '-o', 'code', 'code.c'])

# data sizes 
K = 1024
M = 1024 * 1024
sizes = [1 * K / 4, 100 * K / 4, 1 * M / 4, 8 * M / 4, 32 * M / 4, 64 * M / 4, 128 * M / 4]

num_experiments = 10

hostfile = f"hosts.txt" # The 6 host names are defined here, change the file content to run on different servers
with open(hostfile, 'r') as f:
    host_pairs = f.readlines()
    host_pairs = [(i.split(', ')[0], i.split(', ')[1].strip('\n')) for i in host_pairs]

for size in sizes:
    for host_pair in host_pairs:
        for i in range(num_experiments):
            
            with open('./logs.txt', 'a+') as f:
                print(f"Running {size = }, {host_pair = }, {i = }", file = f)
                
            subprocess.run(['mpirun', '-np', '2', 
                            '-hosts', f'csews{host_pair[0]},csews{host_pair[1]}',
                            './code', str(size)])
