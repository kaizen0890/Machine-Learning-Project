import math
import os
import numpy as np
import matplotlib.pyplot as plt
#np.random.seed()



def calculate_label(w,X):
	return np.sign(X.dot(w))

def algorithm(X,y,w0):
	w = w0
	while True:
		y_new = calculate_label(w,X)
		errorPointIndex = np.where(np.equal(y_new,y)==False)[0]
		numOfErrorPoint = errorPointIndex.shape[0]
		if numOfErrorPoint == 0:
			return w
		newPointIndex = np.random.choice(errorPointIndex, 1)[0]
		w = w + y[newPointIndex]*X[newPointIndex]

def draw_line(w):
    w0, w1, w2 = w[0], w[1], w[2]
    if w2 != 0:
        x11, x12 = -5, 5
        return plt.plot([x11, x12], [-(w1*x11 + w0)/w2, -(w1*x12 + w0)/w2], 'k')
    else:
        x10 = -w0/w1
        return plt.plot([x10, x10], [-5, 5], 'k')


means = [[-1, 0], [1, 0]]
cov = [[.3, .2], [.2, .3]]
N = 10
X0 = np.random.multivariate_normal(means[0], cov, N)
X1 = np.random.multivariate_normal(means[1], cov, N)
X = np.concatenate((X0, X1), axis = 0)
y = np.concatenate((np.ones(N), -1*np.ones(N)))
Xbar = np.concatenate((np.ones((2*N, 1)), X), axis = 1)


w0 = np.random.randn(Xbar.shape[1])
w = algorithm(Xbar,y,w0)



w0, w1, w2 = w[0], w[1], w[2]
x11, x12 = -1, 1

plt.plot(X0[:, 0], X0[:, 1], 'b^', markersize = 4, alpha = .8)
plt.plot(X1[:, 0], X1[:, 1], 'go', markersize = 4, alpha = .8)
plt.plot([x11, x12], [-(w1*x11 + w0)/w2, -(w1*x12 + w0)/w2], 'k')
plt.axis('equal')
plt.plot()


plt.show()