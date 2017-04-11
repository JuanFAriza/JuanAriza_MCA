import numpy as np
import matplotlib.pyplot as plt
import sys
import os
import plotShock as shock

def P(e, rho, u):
    return 0.4*(e - 0.5*rho*u**2)

T = 0.2278
x_an, RHO, U, PR = shock.SolucionExacta(T)

filenames = ''

os.system('mkdir temp')

for i in range(len(RHO)):
    tiempo = T*i/99.0
    command = './euler.x ' + format(tiempo)
    os.system(command)
    data = np.loadtxt('shock.dat')

    U1, U2, U3 = data[1:-1,0], data[1:-1,1], data[1:-1,2]

    x = np.linspace(0, 1, len(U1))
    rho_f = U1
    P_f = P(U3, U1, U2/U1)
    u_f = U2/U1

    rho_an = RHO[i]
    u_an = U[i]
    pr_an = PR[i]

    plt.subplot(311)
    plt.plot(x,rho_f)
    plt.plot(x_an,rho_an)
    plt.ylabel('Density')
    plt.axis([0,1,0,2])

    plt.subplot(312)
    plt.plot(x,u_f)
    plt.plot(x_an,u_an)
    plt.ylabel('Speed')
    plt.axis([0,1,0,2])

    plt.subplot(313)
    plt.plot(x,P_f)
    plt.plot(x_an,pr_an)
    plt.ylabel('Pressure')
    plt.axis([0,1,0,2])

    filename = 'fig_temp_' + format(i) + '.png'
    plt.savefig(filename,format='png')
    plt.close()
    os.system('mv ' + format(filename) + ' temp')

    filenames = filenames + ' ' + filename

    os.system('rm shock.dat')

os.chdir('temp')
command = 'convert -delay 10 -loop 0 ' + filenames + ' movimiento.gif'
os.system(command)
command = 'mv movimiento.gif ..'
os.system(command)
os.chdir('..')
#command = 'rm figs_jfariza10_temp -r'
#os.system(command)
