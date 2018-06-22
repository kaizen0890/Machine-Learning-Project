
import numpy as np
from time import time
from sklearn import neighbors, datasets
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score







"""
Calculate Euclid distance function test
"""

"""


d, N = 100,1000 # dimension, number of training points
X = np.random.randn(N,d) # N d-dimensional points
z = np.random.randn(d)
M = 100
Z = np.random.randn(M,d)


def dis_pp(Z,x):
	d = z - x.reshape(z.shape)
	return np.sum(d*d)
def dis_ps_naive(z,X):
	N = X.shape[0]
	res = np.zeros((1,N))
	for i in range(n):
		res[0][i] = dis_pp(z,X[i])
	print res

def dis_ps_past(z,X):
	X2 = np.sum(X*X,1)
	z2 = np.sum(z*z)
	return X2 + z2 - 2*X.dot(z)

def dis_ss_0(Z,X):
	M = Z.shape[0]
	N = X.shape[0]
	res = np.zeros((M,N))
	for i in range(M):
		res[i] = dis_ps_past(Z[i], X)
	return Z
	
"""

np.random.seed(7)

"""
This is Iris plant dataset which is seperated to 3 classes: Setosa, Versicolour, and Virginica 
Each of class includes 50 samples. Each of sample is describle by 4 attribute information: 
1. sepal length in cm
2. sepal width in cm
3. petal length in cm
4. petal width in cm
5. class: 
	-- Iris Setosa
  -- Iris Versicolour
  -- Iris Virginica

Reference Link:  https://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.names
"""

iris = datasets.load_iris() ## Load iris datasets
iris_X = iris.data ## Dataset 
### Iris_X is matrix with shape [150,4]
iris_y = iris.target ## label of dataset

#print np.unique(iris_y) ## Display all label of array

X_train, X_test, y_train, y_test = train_test_split(iris_X, iris_y, test_size = 130)


model = neighbors.KNeighborsClassifier(n_neighbors = 1, p = 2)

model.fit(X_train, y_train)
y_predict = model.predict(X_test)
print accuracy_score(y_test, y_predict)

#print("Accuracy of 1NN: %.2f %%" %(100*accuracy_score(y_test, y_predict)))












