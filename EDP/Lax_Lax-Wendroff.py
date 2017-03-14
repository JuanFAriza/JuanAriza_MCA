import numpy as np
import matplotlib.pyplot as plt

def u_0(x):
    return 1 - (x/2.0)%1

def F(x):
    return (x**2)/2.0

def Lax(f_0,t_final,delta_x,delta_t):
    Nx = int((4 - 0)/delta_x)
    T = int(t_final/delta_t)
    x = np.linspace(0,4,Nx)
    u = x.copy()
    u = f_0(x)
    ufuturo = u.copy()
    for t in range(T):
        for i in range(1,Nx-1):
            ufuturo[i] = 0.5*(u[i+1] + u[i-1]) - 0.5*(delta_t/delta_x)*(F(u[i+1]) - F(u[i-1]))
        ufuturo[0] = 1
        ufuturo[Nx-1] = 0
        u = ufuturo.copy()
    return x,u


tfinal = [0,0.5,1.0,2.0]
deltax = 0.05
deltat = 0.5*deltax
x,y = Lax(u_0,tfinal[0],deltax,deltat)
plt.plot(x,y)
plt.show()
