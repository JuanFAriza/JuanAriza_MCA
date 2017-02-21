import numpy as np
import matplotlib.pyplot as plt
import csv

file = open('datos.dat', 'rt') # Abre el archivo de datos

try:
    lector = csv.reader(file) # Lee el archivo de datos
    datos = []
    for linea in lector: # Agrega cada linea a la lista de datos
        datos.append(linea)
finally:
    file.close()

medidas = []
for dato in datos:
    medida = dato[0]
    a = medida.split(' ') # Separa por espacios los valores de cada linea
    del a[-1]
    medidas.append(a)

V = np.array(medidas[0:256],dtype='float')
Ex = np.array(medidas[256:512],dtype='float')
Ey = np.array(medidas[512:],dtype='float')

x = np.linspace(0,0.05,256)
y = -x

ax = plt.axes()
ax.set_xlim([0,0.05])
ax.set_ylim([-0.05,0])
ax.set_title('Potencial y campo de placas finitas')

plt.imshow(V,extent=[0,0.05,-0.05,0])
plt.streamplot(x,y,Ex,Ey,density=1.2)

archivo = 'placas.pdf'
plt.savefig(archivo,format='pdf',transparent=True)
