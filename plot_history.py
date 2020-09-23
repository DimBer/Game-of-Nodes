import numpy as np 
import matplotlib.pyplot as plt 


fname = "output/hist1"


D = np.genfromtxt(fname, dtype = int)

plt.subplot(211)
plt.xlabel("Year")
plt.plot(D[:,0], label = "Prey population")
plt.plot(D[:,1], label = "Predator population")
plt.legend()
plt.grid()

plt.subplot(212)
plt.xlabel("Year")
plt.plot(D[:,2], label = "Grass amount")
plt.legend()
plt.grid()

plt.savefig('history_plot.eps')

plt.show()