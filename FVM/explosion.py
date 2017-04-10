import numpy as np
import matplotlib.pyplot as plt

gamma = 1.4 # Gamma de un gas diatomico
L = 250.0
dx = 2.0
Nx = Ny = Nz = L/dx

bombax = bombay = bombaz = (Nx + 1)/2  # Numero de celda de ubicacion de la bomba

U = np.zeros([Nx,Ny,Nz,5]) # U[i][j][k] es un vector de 5 elementos
