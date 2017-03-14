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

l = np.linspace(0.1,16,1000)
prob = posterior(obs,l)
plt.plot(l,prob)
plt.show()
