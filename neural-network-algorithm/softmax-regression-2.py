import os
import math
import numpy as np
import matplotlib.pyplot as plt
np.random.seed(2)

#create number of class and data point

C, N = 5,10
means = [[2, 2], [8, 3], [3, 6], [14, 2], [12, 8]]
cov = [[1, 0], [0, 1]]
X0 = np.random.multivariate_normal(means[0], cov, N)
X1 = np.random.multivariate_normal(means[1], cov, N)
X2 = np.random.multivariate_normal(means[2], cov, N)
X3 = np.random.multivariate_normal(means[3], cov, N)
X4 = np.random.multivariate_normal(means[4], cov, N)

#combine all class data Xi to X matric
X = np.concatenate((X0, X1, X2, X3, X4), axis = 0)
#add bias trick
Xbar = np.concatenate((X, np.ones((X.shape[0],1))), axis = 1)


#Create label matrix y
y = np.asarray([0]*N + [1]*N + [2]*N+ [3]*N + [4]*N)

W_init = np.random.randn(Xbar.shape[1], C)


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


def softmax(Z):
	exp_Z = np.exp(Z)
	A = exp_Z/ exp_Z.sum(axis = 1, keepdims = True)
	return A



def softmax_grad(y, X, W):
	Z = X.dot(W)
	A = softmax(Z)



Z = Xbar.dot(W_init)
A = softmax(Z)

#print np.mean(np.log(A))
id0 = range(Xbar.shape[0]) 

print len(y)
#print -np.mean(np.log(A[id0, y]))
