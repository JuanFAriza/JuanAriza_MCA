import numpy as np
import matplotlib.pyplot as plt
from sklearn import svm
from sklearn import datasets
from sklearn.datasets import make_classification
from sklearn.cross_validation import train_test_split

data = datasets.load_breast_cancer()
print(data.DESCR)

x_train, x_test, y_train, y_test = train_test_split(X, Y, test_size=0.3)
