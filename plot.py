import matplotlib.pyplot as plt
import pandas as pd
import sys
import numpy as np

def plot(ax, name, df, title):
    ax.plot(df["Ts"], df[name], "-", label = title)
    ax.set_xlabel("T")
    ax.set_ylabel(name)

files = sys.argv[1:]

dfs = []
titles = []

fig, axs = plt.subplots(2, 1, figsize = (20, 7))


for i, file in enumerate(files):
    dfs.append(
            pd.read_csv(
                file,
                sep = " ",
                header = 0,
                skiprows = 1,
                dtype = np.float64
                )
            )
    with open(file, 'r') as f:
        titles.append(f.readline())

    plot(axs[0], "Es", dfs[i], titles[i])
    plot(axs[1], "Ms", dfs[i], titles[i])

plt.legend()
plt.show()

