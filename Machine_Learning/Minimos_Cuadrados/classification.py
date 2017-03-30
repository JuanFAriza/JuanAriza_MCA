import numpy as np
import matplotlib.pyplot as plt

datos = np.loadtxt("fake_classification.txt")

datos0 = []
datos1 = []

for i in datos:
    if i[2] == 0:
        datos0.append([i[0],i[1]])
    if i[2] == 1:
        datos1.append([i[0],i[1]])

datos0 = np.array(datos0)
datos1 = np.array(datos1)

x0 = datos0[:,0]
y0 = datos0[:,1]

x1 = datos1[:,0]
y1 = datos1[:,1]

val0, vect0 = np.linalg.eig(np.cov(x0,y0))
val1, vect1 = np.linalg.eig(np.cov(x1,y1))

i0 = np.argmax(val0)
i1 = np.argmax(val1)

print vect1

print vect0.T[i0]
print vect1.T[i1]

eig_vec0 = vect0.T[i0]
eig_vec1 = vect1.T[i1]

plt.scatter(x0,y0,c='r')
plt.scatter(x1,y1,c='b')

t = np.linspace(-1,1,200)
ft = t*eig_vec1[1]/eig_vec1[0]

plt.plot(t+x1.mean(),ft+y1.mean())
ft = t*eig_vec0[1]/eig_vec0[0]

plt.plot(t+x0.mean(),ft+y0.mean())
plt.show()
"""
eigen_vect0 = vect0.T

cambio_base_t = np.linalg.inv(vectors).T
data_cambio = np.matmul(data_B,cambio_base_t)

eigen_vect = np.matmul(eigen_vect_can,cambio_base_t)
"""
