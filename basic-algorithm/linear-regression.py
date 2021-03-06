

import os
import math
import numpy as np
import matplotlib.pyplot as plt



X = np.array([[147, 150, 153, 158, 163, 165, 168, 170, 173, 175, 178, 180, 183]]).T
Y = np.array([49, 50, 51, 54, 58, 59, 60, 62, 63, 64, 66, 67, 68])

#Building Xbar
one = np.ones((X.shape[0], 1))
Xbar = np.concatenate((one, X), axis = 1)


# Calculating weights of the fitting line
A = np.dot(Xbar.T, Xbar)
B = np.dot(Xbar.T, Y)
W = np.dot(np.linalg.pinv(A), B)

w_0, w_1 = W[0], W[1]

active = True
while active:
	name = raw_input("What's your name? ")
	height = float(raw_input("How tall are you?"))
	
	predict = height*w_1 + w_0

	print "Hi " + name 
	print "The computer predict your weight is: " + str(predict) + "kg"

	answer = raw_input("Would you want to try one more time? (yes/no)")
	if answer == 'no':
		active = False

plt.scatter(X, Y)
plt.show()
