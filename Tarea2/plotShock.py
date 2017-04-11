import numpy as np
import matplotlib.pyplot as plt

L = 1.0
xdiscont = 0.5
gamma = 1.4

# Condiciones iniciales
rho_l = 1.0
rho_r = 0.125
u_l = u_r = 0
P_l = 1.0
P_r = 0.1

# Constantes para solucion exacta
a_l = np.sqrt(gamma*P_l/rho_l)
a_r = np.sqrt(gamma*P_r/rho_r)
A_r = 2.0/((gamma + 1)*rho_r)
B_r = P_r*(gamma - 1)/(gamma + 1)

def fR(p):
    val = (p - P_r)*np.sqrt(A_r/(p + B_r))
    return val

def fL(p):
    val = (2.0*a_l/(gamma - 1))*((p/P_l)**(0.5*(gamma - 1)/gamma) - 1)
    return val

def fP(p): # funcion de las condiciones iniciales y presion en la zona central, debe ser 0
    val = fR(p) + fL(p) + u_r - u_l
    return val

def fP_prima(p):
    val = (a_l/(gamma*P_l))*(P_l/p)**((gamma + 1)/(2*gamma)) + np.sqrt(A_r/(p + B_r))*(1 - (p - P_r)/(2*(p + B_r)))
    return val

def SolucionExacta():
    p_c_antes = 0.3
    n = 0
    p_c = p_c_antes - fP(p_c_antes)/fP_prima(p_c_antes)
    #    var = abs(p_c - p_c_antes)/(0.5*(abs(p_c) + abs(p_c_antes)))
    var = abs(fP(p_c))
    
    while (var > 1*(10.0**(-10))):
        p_c_antes = p_c
        p_c = p_c_antes - fP(p_c_antes)/fP_prima(p_c_antes)
        #        var = abs(p_c - p_c_antes)/(0.5*(abs(p_c) + abs(p_c_antes)))
        var = abs(fP(p_c))
        n += 1
        
    u_c = 0.5*(u_l + u_r) + 0.5*(fR(p_c) - fL(p_c))
    rho_c_l = rho_l*((p_c/P_l)**(1/gamma))
    rho_c_r = rho_r*((p_c/P_r) + (gamma - 1)/(gamma + 1))/((gamma - 1)*p_c/(P_r*(gamma + 1)) + 1)
    a_c_l = a_l*((p_c/P_l)**(0.5*(gamma - 1)/gamma))
    S_h_l = u_l - a_l
    S_t_l = u_c - a_c_l
    S_r = u_r + a_r*np.sqrt(0.5*(gamma + 1)*p_c/(gamma*P_r) + 0.5*(gamma - 1)/gamma)

    x = np.linspace(0,1,100)
    t = 0.21
    delta_t = 0.0001
    ud = 0.5
    RHO = x.copy()
    U = x.copy()
    PR = x.copy()

    der_u_max = 0
    while (ud < 0.9):
        t += delta_t
        
        for i in range(len(x)):
            r,u,p = Reg(x[i],t,rho_c_l,rho_c_r,u_c,p_c,S_h_l,S_t_l,S_r)
            RHO[i] = r
            U[i] = u
            PR[i] = p
            der_u = abs(U[i] - U[i-1])
            if (der_u_max < der_u):
                ud = x[i]
                der_u_max = der_u
        der_u_max = 0    
    print t
    plt.plot(x,U)
    plt.show()

def Reg(x,t,rho_c_l,rho_c_r,u_c,p_c,S_h_l,S_t_l,S_r): # Devuelve rho, u, P
    if (x - xdiscont < S_h_l*t): # L
        return rho_l, u_l, P_l
    elif (x - xdiscont < S_t_l*t): # fan
        rho_star = rho_l*((2/(gamma + 1)) + (u_l - (x - xdiscont)/t)*((gamma - 1)/(a_l*(gamma + 1))))**(2/(gamma - 1))
        u_star = (2/(gamma + 1))*(a_l + 0.5*u_l*(gamma - 1) + (x - xdiscont)/t)
        p_star = P_l*((rho_star/rho_l)**gamma)
        return rho_star, u_star, p_star
    elif (x - xdiscont < u_c*t): # L*
        return rho_c_l, u_c, p_c
    elif (x - xdiscont < S_r*t): # R*
        return rho_c_r, u_c, p_c
    else: # R
        return rho_r, u_r, P_r
    
    
SolucionExacta()
