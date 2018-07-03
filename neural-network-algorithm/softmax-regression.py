import numpy as np
import os
import matplotlib.pyplot as plt
from sklearn.datasets import fetch_mldata
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score

path = (os.getcwd())
dataPath = (path+ "/data/")

mnist = fetch_mldata("MNIST original", data_home =dataPath)
X = mnist.data

y = mnist.target


X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=10000)

model = LogisticRegression(C = 1e5,solver = 'lbfgs', multi_class = 'multinomial')
model.fit(X_train,y_train)

y_pred = model.predict(X_test)

errorPointIndex = np.where(np.equal(y_pred,y_test)==False)[0]
N = errorPointIndex.shape[0]

rate = 100.0 - (N*100.0/10000.0)

print "Accuracy: " + str(rate) + " %"
