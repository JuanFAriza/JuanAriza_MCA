import numpy as np
import matplotlib.pyplot as plt
from sklearn.ensemble import RandomForestClassifier

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

arch = open('Dorothea/dorothea_train.labels')
train_labels = np.zeros([800,1],dtype='int')

for i in range(800):
    linea = arch.readline()
    dat_lin = linea.split()
    dat = np.array(dat_lin,dtype='int')
    train_labels[i] = dat[0]

arch = open('Dorothea/dorothea_valid.data')
valid_data = np.zeros([350,100000],dtype='int')

for i in range(350):
    linea = arch.readline()
    dat_lin = linea.split()
    dat = np.array(dat_lin,dtype='int')
    n = 0
    for j in range(100000):
        if (j == dat[n]):
            valid_data[i,j] = 1
            n += 1
            if (n>=len(dat)):
                n = 0

arch = open('Dorothea/dorothea_valid.labels')
valid_labels = np.zeros([350,1],dtype='int')

for i in range(350):
    linea = arch.readline()
    dat_lin = linea.split()
    dat = np.array(dat_lin,dtype='int')
    valid_labels[i] = dat[0]
    
for x in [10, 15, 20, 25, 30, 35, 40, 45]:
    rf = RandomForestClassifier(n_estimators=x)
    rf.fit(train_data,train_labels.T[0])

    predict = rf.predict(valid_data)
    ERR = 1 - np.sum(predict == valid_labels.T[0])/350.0

    ii = np.argsort(rf.feature_importances_)
    uno = ii[-1]
    dos = ii[-2]
    
    print x, 1-ERR, uno, dos


