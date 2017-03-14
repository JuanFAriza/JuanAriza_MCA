import numpy as np
import matplotlib.pyplot as plt

obs = np.array([3.0,5.0,12.0])
a = 1.0
b = 20.0

def verosimilitud(x, l):
    prob = 1
    z = -l*(np.exp(-b/l)-np.exp(-a/l))
    for val in x:
        prob = prob*np.exp(-val/l)/z
    return prob

def prior(l):
    if l < 0 and l > 1000:
        return 0
    return 0.001

def evidencia(x):
    return 1

def posterior(x, l):
    l = np.array(l)
    post = np.zeros(len(l),dtype='float')
    for i in range(len(l)):
        post[i] = verosimilitud(x,l[i])*prior(l[i])/evidencia(x)
    return post

N = 10**6
delta = 0.1
lambda0 = 1.0
lambdas = np.array([lambda0])

def q(x1,x2): # Probabilidad de pasar a x1 dado que estoy en x2
    return 1 # Vamos a asumir que hay la misma probabilidad de saltar a la derecha o a la izquierda

def U(): # Un numero aleatorio que va a decidir si se da el salto o no
    return np.random.rand()

for i in range(N-2):
    lambdanew = lambdas[i] + (np.random.rand() - 0.5)*2*delta # Posible posicion nueva
    alpha = min(1,(posterior(obs,lambdanew)/posterior(obs,lambdas[i]))*(q(lambdanew,lambdas[i])/q(lambdas[i],lambdanew)))
    u = U()
    if (u < alpha):
        lambdas = np.append(lambdas,lambdanew)
    else:
        lambdas = np.append(lambdas,lambdas[i])

plt.hist(lambdas)
plt.show()
