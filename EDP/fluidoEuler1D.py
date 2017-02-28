import numpy as np
import matplotlib.pyplot as plt

gamma = 1.4

def rho_0(x):
    x = np.array(x)
    ii = x<=0.5
    jj = x>0.5
    x[ii] = 1.0
    x[jj] = 0.125, 
    return x


def P_0(x):
    x = np.array(x)
    ii = x<=0.5
    jj = x>0.5
    x[ii] = 1.0
    x[jj] = 0.1
    return x

def u_0(x):
    x = np.array(x)
    x = np.zeros(len(x), dtype='float')
    return x

def e_0(x):
    x = np.array(x)
    e = (P_0(x)/(gamma - 1)) + 0.5*rho_0(x)*u_0(x)*u_0(x)
    return e

def P(e,rho,u):
    e = np.array(e)
    rho = np.array(rho)
    u = np.array(u)
    P = (gamma - 1)*(e - 0.5*rho*u*u)
    return P

def MacCormack(delta_x, delta_t, T):
    t = 0
    Nx = 1.0/delta_x
    x = np.linspace(0,1,Nx)
    U = np.array([x,x,x])
    U[0][:] = rho_0(x)
    U[1][:] = rho_0(x)*u_0(x)
    U[2][:] = e_0(x)
    U0 = U.copy()
    Ufin = U.copy()
    while (t<T):
        rho = U[0][:]
        u = U[1][:]/U[0][:]
        e = U[2][:]
        p = P(e,rho,u)
        F = np.array([u*rho,e*u*u + p,u*(e + p)])

        U_star = U - (delta_t/delta_x)*(np.roll(F,-1,axis=1) - F)
        rho_star = U_star[0][:]
        u_star = U_star[1][:]/U_star[0][:]
        e_star = U_star[2][:]
        p_star = P(e_star,rho_star,u_star)
        F_star = np.array([u_star*rho_star,e_star*u_star*u_star + p_star,u_star*(e_star + p_star)])

        Ufin = 0.5*(U + U_star - (delta_t/delta_x)*(F_star - np.roll(F_star,1,axis=1)))
        U = Ufin.copy()
        t = t + delta_t
    return x, U0, U

deltax = 0.01
deltat = 0.01

T=[1.0,2.0,3.0]

MacCormack(deltax,deltat,T[0])
