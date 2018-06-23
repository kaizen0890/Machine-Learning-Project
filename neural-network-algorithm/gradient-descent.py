
import math
import os

import numpy as np



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

(x1, i1) = grad_decent(5,0.1)

(x2, i2) = grad_decent(-5,0.1)
print str(x1) + " : " + str(i1)
print str(x2) + " : " + str(i2)

