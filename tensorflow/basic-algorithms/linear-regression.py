import tensorflow as tf
import numpy as np 
import matplotlib.image as mp_image
import matplotlib.pyplot as plt

## Create Data
a = 0.22
b = 0.78
N = 500
x_point = []
y_point = []

for i in range(N):
	x = np.random.normal(0,1)
	y = a*x + b + np.random.normal(0,0.2)
	x_point.append(x)
	y_point.append(y)

A = tf.Variable(tf.random_uniform([1], -1.0, 1.0))
B = tf.Variable(tf.zeros([1]))

y = A * x_point + B
cost_function = tf.reduce_mean(tf.square(y - y_point))
optimizer = tf.train.GradientDescentOptimizer(0.2)
train = optimizer.minimize(cost_function)

model = tf.global_variables_initializer()

with tf.Session() as session:
	session.run(model)
	for step in range(21):
		session.run(train)
		if (step % 5 == 0):
			plt.plot(x_point,y_point,'o',label='step = {}'.format(step))
			plt.plot(x_point,session.run(A) *x_point + session.run(B))
			plt.legend()
			plt.show()

