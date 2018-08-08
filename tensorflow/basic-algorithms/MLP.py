import numpy as np 
import tensorflow as tf 
import matplotlib.pyplot as plt 
from tensorflow.examples.tutorials.mnist import input_data

mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)


## Create graph input 
x = tf.placeholder("float", [None, 784])
y = tf.placeholder("float", [None, 10])


## Define parametters of model
input_node = 784 #
hid_layer_1 = 256
hid_layer_2 = 256
output_node = 10

learning_rate = 0.001
epoches = 20
batch_size = 100
display_step = 1


#Plot settings
avg_set = []
epoch_set=[]


################# L A Y E R   1 ##################
## Create W1, b1 
W_1 = tf.Variable(tf.random_normal([input_node,hid_layer_1])) #Create W_1 with size[784,256]
b_1 = tf.Variable(tf.random_normal([hid_layer_1]))

## Calculate z and activation function of layer 1
z_1 = tf.add(tf.matmul(x,W_1), b_1)
a_1 = tf.nn.sigmoid(z_1)

################# L A Y E R   2 ##################
## Create W1, b1 
W_2 = tf.Variable(tf.random_normal([hid_layer_1,hid_layer_2]))
b_2 = tf.Variable(tf.random_normal([hid_layer_2]))

## Calculate z and activation function of layer 2
z_2 = tf.add(tf.matmul(a_1,W_2),b_2)
a_2 = tf.nn.sigmoid(z_2)

#################  O U T P U T      L A Y E R  ##################
## Create W1, b1 
W_output = tf.Variable(tf.random_normal([hid_layer_2,output_node]))
b_output = tf.Variable(tf.random_normal([output_node]))

## Calculate z and activation function of layer 2
output = tf.add(tf.matmul(a_2,W_output), b_output)

#################  C O S T    &    O P T M I Z E R  ##################

## Construct cost function
cost_function = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(logits = output, labels = y))

## Construct optimizer
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost_function)


#################  T R A I N    &    T E S T  ##################


## Initialize model
MLP_model = tf.global_variables_initializer()

with tf.Session() as session:
	session.run(MLP_model)
	for epoch in range(epoches):
		avg_cost = 0.

		total_batches = int(mnist.train.num_examples/batch_size)

		for batch in range(total_batches):
			train_data,train_labels = mnist.train.next_batch(batch_size)
			session.run(optimizer, feed_dict ={x:train_data, y:train_labels})
			avg_cost += session.run(cost_function,feed_dict={x:train_data, y:train_labels})/total_batches
		if epoch % display_step == 0:
			print "Epoch:", '%04d' % (epoch+1),"cost=", "{:.9f}".format(avg_cost)
		avg_set.append(avg_cost)
		epoch_set.append(epoch+1)

	plt.plot(epoch_set,avg_set, 'o',label='Softmax Regression training phase')
	plt.ylabel('cost')
	plt.xlabel('epoch')
	plt.legend()
	plt.show()

	## Construct compare function between testing labels and true labels
	correct_prediction = tf.equal(tf.argmax(output, 1),tf.argmax(y, 1))

	## Construct function which calculate accuracy of model
	accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))

	## Test data with mnist.test.image and mnist.test.labels
	result = accuracy.eval(session=session,feed_dict = {x: mnist.test.images,y: mnist.test.labels})
	print
	print "Accuracy of testing model is:", result







