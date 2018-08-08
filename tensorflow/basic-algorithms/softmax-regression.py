import tensorflow as tf 
import matplotlib.pyplot as plt 
import numpy as np 
from tensorflow.examples.tutorials.mnist import input_data

mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)


## Create graph input
x = tf.placeholder("float",[None, 784])
y = tf.placeholder("float", [None, 10])

## Create Model weight
W = tf.Variable(tf.zeros([784,10]))
b = tf.Variable(tf.zeros([10]))

## Create parametter
learning_rate = 0.2
batch_size = 100
training_epoches = 25
display_step = 1

#Plot settings
avg_set = []
epoch_set=[]

## Construct activation function
softmax_function = tf.nn.softmax(tf.matmul(x,W) + b)

## Construct cost function
cost_function = tf.reduce_mean(-tf.reduce_sum(y*tf.log(softmax_function),reduction_indices = 1))

## Construct optimizer for cost function
optimizer = tf.train.GradientDescentOptimizer(learning_rate).minimize(cost_function)

## Initialize model
model = tf.global_variables_initializer()

with tf.Session() as session:
	session.run(model)
	for epoch in range(training_epoches):
		avg_cost = 0.
		total_batches = int(mnist.train.num_examples/batch_size)
		for batch in range(total_batches):
			train_data,train_labels = mnist.train.next_batch(100)
			session.run(optimizer, feed_dict ={x:train_data, y:train_labels})
			avg_cost += session.run(cost_function,feed_dict={x:train_data, y:train_labels})/total_batches
		if epoch % display_step == 0:
			print "Epoch:", '%04d' % (epoch+1),"cost=", "{:.9f}".format(avg_cost)
		avg_set.append(avg_cost)
		epoch_set.append(epoch+1)

	plt.plot(epoch_set,avg_set, 'o',label='Logistic Regression Training phase')
	plt.ylabel('cost')
	plt.xlabel('epoch')
	plt.legend()
	plt.show()

	## Construct compare function between testing labels and true labels
	correct_prediction = tf.equal(tf.argmax(softmax_function, 1),tf.argmax(y, 1))

	## Construct function which calculate accuracy of model
	accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))

	## Test data with mnist.test.image and mnist.test.labels
	result = accuracy.eval(session=session,feed_dict = {x: mnist.test.images,y: mnist.test.labels})
	print
	print "Accuracy of testing model is:", result



