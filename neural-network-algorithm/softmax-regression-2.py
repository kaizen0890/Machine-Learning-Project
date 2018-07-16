import os
import math
import numpy as np
import matplotlib.pyplot as plt
from scipy import sparse 



def convert_labels(y, C):
	Y = sparse.coo_matrix((np.ones_like(y), (y, np.arange(len(y)))), shape = (C, len(y))).toarray()
	return Y 

def softmax(Z):
	e_Z = np.exp(Z)
	A = e_Z / e_Z.sum(axis = 0)
	return A

def softmax_stable(Z):
	e_Z = np.exp(Z - np.max(Z, axis = 0, keepdims = True))
	A = e_Z / e_Z.sum(axis = 0)
	return A

def cost(X, Y, W):
	A = softmax(W.T.dot(X))
	return -np.sum(Y*np.log(A))

def grad(X, Y, W):
	A = softmax((W.T.dot(X)))
	E = A - Y
	return X.dot(E.T)

def softmax_regression(X, y, W_init, eta, tol = 1e-4, max_count = 10000):
	W = [W_init]    
	C = W_init.shape[1]
	Y = convert_labels(y, C)
	it = 0
	N = X.shape[1]
	d = X.shape[0]
	count = 0
	check_w_after = 20
	while count < max_count:
		mix_id = np.random.permutation(N)
		for i in mix_id:
			xi = X[:, i].reshape(d, 1)
			yi = Y[:, i].reshape(C, 1)
			ai = softmax(np.dot(W[-1].T, xi))
			W_new = W[-1] + eta*xi.dot((yi - ai).T)
			count += 1
			if count%check_w_after == 0:                
				if np.linalg.norm(W_new - W[-check_w_after]) < tol:
					return W
			W.append(W_new)
	return W

means = [[2, 2], [8, 3], [3, 6]]
cov = [[1, 0], [0, 1]]
N = 500
X0 = np.random.multivariate_normal(means[0], cov, N)
X1 = np.random.multivariate_normal(means[1], cov, N)
X2 = np.random.multivariate_normal(means[2], cov, N)

X = np.concatenate((X0, X1, X2), axis = 0).T # each column is a datapoint
X = np.concatenate((np.ones((1, 3*N)), X), axis = 0)
C = 3

original_label = np.asarray([0]*N + [1]*N + [2]*N).T

eta = .05 
W_init = np.random.randn(X.shape[0], C)
W = softmax_regression(X, original_label, W_init, eta)

print W[-1]



































"""
# Display data point on matplot
x = range(-20,20)
y = range(-20,20)
fig = plt.figure()
ax1 = fig.add_subplot(111)
ax1.scatter(X0[:,[0]], X0[:,[1]], s=10, c='b', marker="s", label='first')
ax1.scatter(X1[:,[0]], X1[:,[1]], s=10, c='g', marker="s", label='second')
ax1.scatter(X2[:,[0]], X2[:,[1]], s=10, c='r', marker="s", label='third')
ax1.scatter(X3[:,[0]], X3[:,[1]], s=10, c='y', marker="s", label='fourth')
ax1.scatter(X4[:,[0]], X4[:,[1]], s=10, c='c', marker="s", label='fifth')
#plt.show()
"""


