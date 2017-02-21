import numpy as np
import matplotlib.pyplot as plt

def u_0(x):
    if (x <= 50) or (x >= 110):
        return 0
    else:
        return 100*np.sin(np.pi*(x-50)/60)

def frontera(x):
    if (x == 0) or (x == 300):
        return 0

def FTFS(f_0,a,t_max,delta_x,delta_t):
    T = int(t_max/delta_t)
    Nx = int((300 - 0)/delta_x)
    x = np.linspace(0,300,Nx)
    u = np.copy(x)
    u = np.array([u,u])
    for i in range(Nx):
        u[1,i] = f_0(x[i])
    y = u[1,:]
    for j in range(T):
        for i in range(1,Nx-2):
            u[0,i] = u[1,i] - a*(delta_t/delta_x)*(u[1,i+1]-u[1,i])
        u[0,0] = u[0,Nx-1] = 0
        u = np.roll(u,1,axis=0)
    return x,u[0,:],y

def FTCS(f_0,a,t_max,delta_x,delta_t):
    T = int(t_max/delta_t)
    Nx = int((300 - 0)/delta_x)
    x = np.linspace(0,300,Nx)
    u = np.copy(x)
    ufinal = np.copy(u)
    for i in range(Nx):
        u[i] = f_0(x[i])
    u0 = u.copy()
    for j in range(T):
        for i in range(1,Nx-1):
            ufinal[i] = u[i] - a*(delta_t/(2*delta_x))*(u[i+1] - u[i-1])
        ufinal[0] = ufinal[-1] = 0
        u = ufinal.copy()
    return x,u,u0

x,y,yinit = FTCS(u_0,30,0.45,5.0,0.000015)
plt.plot(x,y)
plt.plot(x,yinit)
plt.show()
