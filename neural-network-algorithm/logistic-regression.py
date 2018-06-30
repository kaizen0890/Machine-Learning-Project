
import math
import os
import numpy as np
import matplotlib.pyplot as plt
np.random.seed(2)

################# Training #################

def sigmoid(S):
	return 1/(1 + np.exp(-S))
def calculate_Zi(w,X):
	return sigmoid(X.dot(w))

def loss_function(w, X, y):
	z = calculate_Zi(w, X)
	return -np.mean(y*np.log(z) + (1-y)*np.log(1-z))


def algorithm(w_init, X, y, eta = 0.1, nepoches = 2000):
	N, d = X.shape[0], X.shape[1]
	w = w_old = w_init
	ep = 0
	loss_array = []
	loss_array.append(loss_function(w_init,X,y))
	while ep < nepoches:
		ep+=1
		mix_index = np.random.permutation(N)
		for i in mix_index:
			xi = X[i]
			yi = y[i]
			zi = sigmoid(xi.dot(w))
			w = w - eta*((zi - yi)*xi)
		loss_array.append(loss_function(w,X,y))

		if np.linalg.norm(w - w_old)/d < 1e-6:
			break
		w_old = w
	return w , loss_array


X = np.array([[0.50, 0.75, 1.00, 1.25, 1.50, 1.75, 1.75, 2.00, 2.25, 2.50,
2.75, 3.00, 3.25, 3.50, 4.00, 4.25, 4.50, 4.75, 5.00, 5.50]]).T
y = np.array([0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1])
# bias trick
Xbar = np.concatenate((X, np.ones((X.shape[0], 1))), axis = 1)

w_init = np.random.randn(Xbar.shape[1])
eta = 0.05
w, loss_array = algorithm(w_init, Xbar, y, eta, nepoches = 500)

w0 = w[0]
w1 = w[1]
print "w[0] = " + str(w0)
print "w[1] = " + str(w1)
print "Final loss function value = " + str(loss_array[-1])
print "Thresh hold = " + str(-w0/w1)

############ Testing #############
"""
probability_of_pass = sigmoid(w[0] * X_data + w[1])

""" 
xx = np.linspace(0, 6, 1000)
threshold = -w0/w1
yy = sigmoid(w0 + w1*xx)
plt.axis([-2, 8, -1, 2])
plt.plot(xx, yy, 'g-', linewidth = 2)
plt.plot(threshold, .5, 'y^', markersize = 8)
plt.xlabel('studying hours')
plt.ylabel('predicted probability of pass')
plt.show()