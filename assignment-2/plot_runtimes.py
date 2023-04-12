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
    
    bp1 = plt.boxplot(boxplot_1d_runtimes, positions=[1,2,1,2], patch_artist=True, boxprops=dict(facecolor="C0"))
    bp2 = plt.boxplot(boxplot_2d_runtimes, positions=[1,2,1,2], patch_artist=True, boxprops=dict(facecolor="C2"))
    
    plt.xticks(ticks = range(3), labels = ["", (2,2), (2,4)], rotation = 30)  # required to fix label alignment issue
    plt.xlabel("Configurations")
    plt.ylabel("Runtime in seconds")
    plt.legend([bp1["boxes"][0], bp2["boxes"][0]], ['1D', '2D'], bbox_to_anchor = (1,1))
    plt.title("Configuration Vs Runtime")
    plt.tight_layout()
    
    plt.savefig('./runtimes.png', dpi=300)
