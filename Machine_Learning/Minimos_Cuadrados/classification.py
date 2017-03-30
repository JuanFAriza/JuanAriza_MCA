import numpy as np
import matplotlib.pyplot as plt

def probGauss(dat, vect, val, mean):
    dat = dat - mean
    dat = vect.T.dot(dat)
    return np.sqrt(val[0]*val[1])*np.exp(-(1/val[0])*(dat[0])**2 - (1/val[1])*(dat[1])**2)    

datos = np.loadtxt("fake_classification.txt")

datos0 = []
datos1 = []

for i in datos:
    if i[2] == 0:
        datos0.append(i)
    if i[2] == 1:
        datos1.append(i)

datos0 = np.array(datos0)
datos1 = np.array(datos1)

def CalcularError(prop):
    datos0tr = datos0[:int(prop*len(datos0))]
    datos0ts = datos0[int(prop*len(datos0)):]

    datos1tr = datos1[:int(prop*len(datos1))]
    datos1ts = datos1[int(prop*len(datos1)):]

    x0 = datos0tr[:,0]
    y0 = datos0tr[:,1]

    x1 = datos1tr[:,0]
    y1 = datos1tr[:,1]

    val0, vect0 = np.linalg.eig(np.cov(x0,y0))
    val1, vect1 = np.linalg.eig(np.cov(x1,y1))

    mean0 = np.array([x0.mean(),y0.mean()])
    mean1 = np.array([x1.mean(),y1.mean()])

    errores = 0

    for i in datos0ts:
        prob = np.array([])
        prob = np.append(prob,probGauss(i[:2],vect0,val0,mean0))
        prob = np.append(prob,probGauss(i[:2],vect1,val1,mean1))
        Exp = np.argmax(prob)
        if 0 != Exp:
            errores += 1

    for i in datos1ts:
        prob = np.array([])
        prob = np.append(prob,probGauss(i[:2],vect0,val0,mean0))
        prob = np.append(prob,probGauss(i[:2],vect1,val1,mean1))
        Exp = np.argmax(prob)
        if 1 != Exp:
            errores += 1

    return (errores+0.0)/(len(datos0ts) + len(datos1ts))

t = np.linspace(0.1,0.9,100)
y = t.copy()
for i in range(len(y)):
    y[i] = CalcularError(y[i])

plt.plot(t,y)
plt.show()

"""  Esto es para mostrar componentes principales
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
