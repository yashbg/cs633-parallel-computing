import matplotlib.pyplot as plt
import numpy as np

time_file = './times.txt'

K = 1024
M = 1024 * 1024
sizes = [1 * K / 4, 100 * K / 4, 1 * M / 4, 8 * M / 4, 32 * M / 4, 64 * M / 4, 128 * M / 4]
sizes = [int(i) for i in sizes]

num_experiments = 10

hostfile = f"hosts.txt" # The 6 host names are defined here, change the file content to run on different servers
with open(hostfile, 'r') as f:
    host_pairs = f.readlines()
    host_pairs = [(i.split(', ')[0], i.split(', ')[1].strip('\n')) for i in host_pairs]

label_sizes = []
for i, size in enumerate(sizes):
    if i < 2:
        label_sizes.append(f"{size/K} KB")
    else :
        label_sizes.append(f"{size/M} MB")

with open(time_file, 'r') as f:
    times = f.readlines()

line_counter = 0
times_dict = {}
for size in sizes:
    times_dict[size] = {}
    for i, host_pair in enumerate(host_pairs):
        time_taken = times[line_counter : line_counter + num_experiments]
        time_taken = [float(i) for i in time_taken]
        line_counter = line_counter + num_experiments
        
        times_dict[size][i] = time_taken

mean_list, std_list = [], []
for size in sizes:
    data = np.array(list(times_dict[size].values()))
    mean, std = np.mean(data).item(), np.std(data).item()
    mean_list.append(mean)
    std_list.append(std)

plt.errorbar(np.arange(len(sizes)), y = mean_list, yerr = std_list)
plt.xlabel('Data size')
plt.xticks(np.arange(len(sizes)), label_sizes)
plt.ylabel('Time taken in seconds')
plt.title(f'Data size vs Transfer time (x-axis scaled)')
plt.savefig(f'./plots/data_vs_time_scaled.png', dpi = 300)
plt.close()

plt.errorbar(sizes, y = mean_list, yerr = std_list)
plt.xlabel('Data size in bytes')
plt.ylabel('Time taken in seconds')
plt.title(f'Data size vs Transfer time')
plt.savefig(f'./plots/data_vs_time.png', dpi = 300)
plt.close()
