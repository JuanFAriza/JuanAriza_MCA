import numpy as np
import matplotlib.pyplot as plt ##

N = 20

datos = np.loadtxt('fake_regression.txt')

training = datos[:int(0.75*len(datos))]
test = datos[int(0.75*len(datos)):]

x = training[:,0].copy()
y = training[:,1].copy()

x_test = test[:,0].copy()
y_test = test[:,1].copy()

Errores = np.array([])

for i in range(1,N):
    beta = np.polyfit(x,y,i)
    y_est = np.zeros(len(x))
    for j in range(i):
        y_est = y_est + beta[j]*(x**(i-j))
    err = (y - y_est)**2
    ECP_tr = err.mean()
    y_est = np.zeros(len(x_test))    
    for j in range(i):
        y_est = y_est + beta[j]*(x_test**(i-j))
    err = (y_test - y_est)**2
    ECP_test = err.mean()

    Error = ECP_tr + ECP_test
    Errores = np.append(Errores, Error)

print Errores
print "Polinomio de mejor ajuste es de grado : " + format(np.argmin(Errores) + 1)
