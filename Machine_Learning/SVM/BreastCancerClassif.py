import numpy as np
import matplotlib.pyplot as plt
from sklearn import svm
from sklearn import datasets
from sklearn.datasets import make_classification
from sklearn.model_selection import train_test_split

data = datasets.load_breast_cancer()
