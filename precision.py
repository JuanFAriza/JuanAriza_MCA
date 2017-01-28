import matplotlib.pyplot as plt
import numpy as np

x, y = np.loadtxt("precision.dat").T

plt.plot(x, y)
plt.xscale("log")
plt.yscale("log")
plt.show()
