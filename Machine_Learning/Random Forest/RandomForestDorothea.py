import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.ensemble import RandomForestClassifier

import sys
import csv

param = pd.read_csv('Dorothea/dorothea_test.data')
test_data = pd.read_csv('Dorothea/dorothea_test.data')
train_data = pd.read_csv('Dorothea/dorothea_train.data')
train_labels = pd.read_csv('Dorothea/dorothea_train.labels')

arch = open('Dorothea/dorothea_train.data')
train_data = np.zeros([800,100000],dtype='int')

for i in range(800):
    linea = arch.readline()
    dat_lin = linea.split()
    dat = np.array(dat_lin,dtype='int')
    n = 0
    for j in range(100000):
        if (j == dat[n]):
            train_data[i,j] = 1
            n += 1
            if (n>=len(dat)):
                n = 0
        else:
            train_data[i,j] = 0

arch = open('Dorothea/dorothea_train.labels')
train_labels = np.zeros([800,1],dtype='int')

for i in range(800):
    linea = arch.readline()
    dat_lin = linea.split()
    dat = np.array(dat_lin,dtype='int')
    train_labels[i] = dat[0]
    
rf = RandomForestClassifier(n_estimators=50)
rf.fit(train_data,train_labels.T[0])
