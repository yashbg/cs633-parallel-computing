import numpy as np
import matplotlib.pyplot as plt
import re

def add_entry(runtime_dict, px, py, size, time):
    if (px, py, size) not in runtime_dict:
        runtime_dict[(px, py, size)] = []
    runtime_dict[(px, py, size)].append(time)

if __name__ == '__main__':
    with open('./times.txt', 'r') as f:
        content = f.readlines()
    
    runtimes1D = {}   
    runtimes2D = {}
    for line in content:
        result = re.search(r"configuration px = (\d*), py = (\d*), size = (\d*), runtime 1D = (\d*.?\d*), runtime 2D = (\d*.?\d*)", line)
        px, py, size, runtime1D, runtime2D = [float(i) for i in result.groups()]
        
        add_entry(runtimes1D, px, py, size, runtime1D)
        add_entry(runtimes2D, px, py, size, runtime2D)

    boxplot_1d_runtimes = []
    boxplot_2d_runtimes = []
    for i, (config, val) in enumerate(runtimes1D.items()):
        boxplot_1d_runtimes.append(val)
    
    for i, (config, val) in enumerate(runtimes2D.items()):
        boxplot_2d_runtimes.append(val)
        
    plt.boxplot(boxplot_1d_runtimes)
    plt.boxplot(boxplot_2d_runtimes)
    
    plt.xticks(ticks = range(len(runtimes1D)+1), labels = [""] + [(px, py) for px, py, size in runtimes1D], rotation = 30)  # required to fix label alignment issue
    plt.xlabel("Configurations")
    plt.ylabel("Runtime in seconds")
    plt.legend(bbox_to_anchor = (1,1))
    plt.title("Configuration Vs Runtime")
    plt.tight_layout()
    
    plt.savefig('./runtimes.png', dpi=300)
