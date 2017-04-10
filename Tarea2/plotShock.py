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

def fP(p): # funcion de las condiciones iniciales y presion en la zona central, debe ser 0
    val = (2.0*a_l/(gamma - 1))*((p/P_l)**((gamma - 1)/2.0*gamma) - 1) + (p - P_r)*np.sqrt(A_r/(p + B_r)) + u_r - u_l
    return val

def SolucionExacta():
    p_c = (P_l - P_r)/2
    
