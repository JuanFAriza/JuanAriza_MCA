import numpy as np
import matplotlib.pyplot as plt

L=1
gamma = 1.4

# Condiciones iniciales
rho_l = 1
rho_r = 0.125
u_l = u_r = 0
P_l = 1
P_r = 0.1

# Constantes para solucion exacta
a_l = np.sqrt(gamma*P_l/rho_l)
A_r = 2.0/((gamma + 1)*rho_r)
B_r = P_r*(gamma - 1)/(gamma + 1)

def fR(p):
    val = (p - P_r)*np.sqrt(A_r/(p + B_r))
    return val

def fL(p):
    val = (2.0*a_l/(gamma - 1))*((p/P_l)**((gamma - 1)/2.0*gamma) - 1)
    return val

def fP(p): # funcion de las condiciones iniciales y presion en la zona central, debe ser 0
#    val = (2.0*a_l/(gamma - 1))*((p/P_l)**((gamma - 1)/2.0*gamma) - 1) + (p - P_r)*np.sqrt(A_r/(p + B_r)) + u_r - u_l
    val = fR(p) + fL(p) + u_r - u_l
    return val

def fP_prima(p):
    val = (a_l/(gamma*P_l))*(P_l/p)**((gamma + 1)/(2*gamma)) + np.sqrt(A_r/(p + B_r))*(1 - (p - P_r)/(2*(p + B_r)))
    return val

def SolucionExacta():
    p_c_antes = (P_l - P_r)/2
    n = 0
    p_c = p_c_antes - fP(p_c_antes)/fP_prima(p_c_antes)
    var = abs(p_c - p_c_antes)/(0.5*(abs(p_c) + abs(p_c_antes)))
    while (var > 0.14*(10.0**(-15))):
        p_c_antes = p_c
        p_c = p_c_antes - fP(p_c_antes)/fP_prima(p_c_antes)
        var = abs(p_c - p_c_antes)/(0.5*(abs(p_c) + abs(p_c_antes)))
        n += 1
    u_c = 0.5*(u_l + u_r) + 0.5*(fR(p_c) - fL(p_c))
    rho_c_l = rho_l*((p_c/P_l)**(1/gamma))
    rho_c_r = rho_r*((p_c/P_r) + (gamma - 1)/(gamma + 1))/((gamma - 1)*p_c/(P_r*(gamma + 1)) + 1)
    print n
    print p_c
    print u_c
    print rho_c_l
    print rho_c_r

SolucionExacta()
