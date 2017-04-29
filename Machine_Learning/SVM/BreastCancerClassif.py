import numpy as np
import matplotlib.pyplot as plt
from sklearn import svm
from sklearn import datasets
from sklearn import preprocessing
from sklearn.metrics import confusion_matrix
from sklearn.datasets import make_classification
from sklearn.cross_validation import train_test_split

data = datasets.load_breast_cancer()
#print(data.DESCR)

X = data['data']
Y = data.target

#scalar = preprocessing.StandardScaler().fit(X)
#X = scalar.transform(X)

x_train, x_valid, y_train, y_valid = train_test_split(X, Y, test_size=0.2)
x_train, x_test, y_train, y_test = train_test_split(x_train, y_train, test_size=0.3)
"""
for C in np.linspace(0,10,10):
    if C != 0:
        linear_svc = svm.SVC(C=C, kernel='linear')
        linear_svc.fit(x_train, y_train)

        y_predict = linear_svc.predict(x_test)

        print "linear C=", C
        print confusion_matrix(y_test, y_predict)

C=1.0
linear_svc = svm.SVC(C=C, kernel='linear')
linear_svc.fit(x_train, y_train)

y_predict = linear_svc.predict(x_test)

print "linear C=", C
print confusion_matrix(y_test, y_predict)

print " "
print " "
print " "
"""
d = 2
for C in np.linspace(0,1.5,10):
    if C != 0:
        poly_svc = svm.SVC(C=C, kernel='poly', degree=d)
        poly_svc.fit(x_train, y_train)

        y_predict = poly_svc.predict(x_test)

        print "poly C=", C, " d=", d
        print confusion_matrix(y_test, y_predict)

C = 1
d = 2
poly_svc = svm.SVC(C=C, kernel='poly', degree=d)
poly_svc.fit(x_train, y_train)

y_predict = poly_svc.predict(x_test)

print "poly C=", C, " d=", d
print confusion_matrix(y_test, y_predict)
