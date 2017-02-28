import numpy as np
import matplotlib.pyplot as plt

def eliminacionGaussiana(matA,vectB):
    if len(matA) == len(vectB):
        for j in range(len(matA)):
            var = matA[j,j]
            matA[j] = (1.0/var)*matA[j]
            vectB[j] = (1.0/var)*vectB[j]
            for i in range(len(matA)):
                if i != j:
                    var = matA[i,j]
                    matA[i] = -var*matA[j]+matA[i]
                    vectB[i] = -var*vectB[j]+vectB[i]
        return vectB

A = np.array([[(1/3.0),(1/4.0),(1/5.0)],[(1/4.0),(1/5.0),(1/6.0)],[(1/5.0),(1/6.0),(1/7.0)]])
b = np.array([0.218296,0.141478,0.103715])

a = eliminacionGaussiana(A,b)

x = np.linspace(0,1,100)

phi = np.exp(-x)*(x)**(1/3.0)

phi_gorro = x*a[0] + x*x*a[1] + x*x*x*a[2]

plt.plot(x,phi)
plt.plot(x,phi_gorro)
plt.show()
