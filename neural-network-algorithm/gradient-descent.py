
import math
import os
import numpy as np

"""
Gradient for one variable function
"""

"""

def grad (x):
	return 2*x + 5*np.cos(x)

def func_value(x):
	return x**2 + 5*np.sin(x)

def grad_decent(x0, eta):
	x = [x0]
	for it in range(100):
		x_new = x[-1] - eta*grad(x[-1])
		if abs(grad(x_new)) < 1e-3: 
			break
	x.append(x_new)
	return (x, it)
def single_variable_GD():
	(x1, i1) = grad_decent(5,0.1)
	(x2, i2) = grad_decent(-5,0.1)
	print str(x1) + " : " + str(i1)
	print str(x2) + " : " + str(i2)
"""


#Gradient Decent (GD) for multi variable function

from sklearn.linear_model import LinearRegression
X = np.random.rand(1000)
y = 4 + 3 * X + .5*np.random.randn(1000) # noise added
model = LinearRegression()
model.fit(X.reshape(-1, 1), y.reshape(-1, 1))
w, b = model.coef_[0][0], model.intercept_[0]
sol_sklearn = np.array([b, w])


def grad(w):
	N = Xbar.shape[0]
	return (Xbar.T.dot(Xbar.dot(w) - y))/N

def cost(w):
	N = Xbar.shape[0]
	return .5/N*np.linalg.norm(y - Xbar.dot(w))**2


######## Basic Gradient Descent #########

def basic_GD(w_init, grad, eta):
	w = [w_init]
	for it in range(100):
		w_new = w[-1] - eta*grad(w[-1])
		if np.linalg.norm(grad(w_new))/len(w_new) < 1e-3:
			break
		w.append(w_new)
	return (w, it)

######## GD with momentum #########

def GD_momentum(grad, theta_init, eta, gamma):
	# Suppose we want to store history of theta
	theta = [theta_init]
	v_old = np.zeros_like(theta_init)
	for it in range(100):
		v_new = gamma*v_old + eta*grad(theta[-1])
		theta_new = theta[-1] - v_new
		if np.linalg.norm(grad(theta_new))/np.array(theta_init).size < 1e-3:
			break
		theta.append(theta_new)
		v_old = v_new
	return theta

######## Nesterov Accelerated Gradient #########
def GD_NAG(grad, theta_init, eta, gamma):
	theta = [theta_init]
	v = [np.zeros_like(theta_init)]
	for it in range(100):
		v_new = gamma*v[-1] + eta*grad(theta[-1] - gamma*v[-1])
		theta_new = theta[-1] - v_new
		if np.linalg.norm(grad(theta_new))/np.array(theta_init).size < 1e-3:
			break
		theta.append(theta_new)
		v.append(v_new)
	return theta


one = np.ones((X.shape[0],1))
Xbar = np.concatenate((one, X.reshape(-1, 1)), axis = 1)
w_init = np.array([[2], [1]])
(w1, it1) = basic_GD(w_init, grad, 1)






	







