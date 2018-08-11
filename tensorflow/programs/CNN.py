import numpy as np 
import tensorflow as tf 
import matplotlib.pyplot as plt 
from tensorflow.examples.tutorials.mnist import input_data

mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)


## Define parametter
learning_rate = 0.001
training_iters = 100000
batch_size = 100
display_step = 10
dropout = 0.75

## Network parametter
n_input = 784
n_output = 10

## Graph input
x = tf.placeholder(tf.float32, [None, n_input])
y = tf.placeholder(tf.float32, [None, n_output])
keep_prob = tf.placeholder(tf.float32)


######  Define necessary functions ######

def conv2d(image, W, b, strides = 1):
	conv = tf.nn.conv2d(image, W, strides=[1, strides, strides, 1], padding='SAME')
	return tf.nn.relu(tf.nn.bias_add(conv, b))
def max_pooling(x, k=2):
	return tf.nn.max_pool(x, ksize=[1, k, k, 1], strides=[1, k, k, 1],padding='SAME')

#########   Construct Weight and bias tensor   ########## 
## layer 1
conv_W_1 = tf.Variable(tf.random_normal([5,5,1,32]))
conv_b_1 = tf.Variable(tf.random_normal([32]))
## layer 2
conv_W_2 = tf.Variable(tf.random_normal([5,5,32,64]))
conv_b_2 = tf.Variable(tf.random_normal([64]))


## layer 3
conv_W_3 = tf.Variable(tf.random_normal([5,5,64,128]))
conv_b_3 = tf.Variable(tf.random_normal([128]))

## Fully connected layer
conv_W_dense = tf.Variable(tf.random_normal([4*4*128,1024]))
conv_b_dense = tf.Variable(tf.random_normal([1024]))
## output layer
conv_W_output = tf.Variable(tf.random_normal([1024, n_output]))
conv_b_output = tf.Variable(tf.random_normal([10]))


## Construct model
X_tensor = tf.reshape(x , shape = [-1,28,28,1])

#########   C N N   F O R    L A Y E R   1   ##########
## Convolution mutiply input image with W1
conv_L1 = conv2d(X_tensor, conv_W_1,conv_b_1, 1)
## Max pooling step
conv_L1 = max_pooling(conv_L1, k=2)
## Decrease overfitting by dropout
conv_L1_output = tf.nn.dropout(conv_L1,keep_prob)


#########   C N N   F O R    L A Y E R   2   ##########
## Convolution mutiply input image with W1
conv_L2 = conv2d(conv_L1_output, conv_W_2,conv_b_2, 1)
## Max pooling step
conv_L2 = max_pooling(conv_L2, k=2)
## Decrease overfitting by dropout
conv_L2_output = tf.nn.dropout(conv_L2,keep_prob)


#########   C N N   F O R    L A Y E R   3   ##########
## Convolution mutiply input image with W1
conv_L3 = conv2d(conv_L2_output, conv_W_3,conv_b_3, 1)
## Max pooling step
conv_L3 = max_pooling(conv_L3, k=2)
## Decrease overfitting by dropout
conv_L3_output = tf.nn.dropout(conv_L3,keep_prob)


#########   F U L L Y    C O N N E C T    L A Y E R   ##########
## Reshape output of conv layers to fully connected layer
dense1 = tf.reshape(conv_L3_output, [-1, conv_W_dense.get_shape().as_list()[0]])
## ReLU activation function
dense1 = tf.nn.relu(tf.add(tf.matmul(dense1, conv_W_dense),conv_b_dense))
# Apply Dropout
dense_output = tf.nn.dropout(dense1, keep_prob)


#########   P R E D I C T I O N   ##########

pred = tf.add(tf.matmul(dense_output, conv_W_output) , conv_b_output)

#########   C O S T    F U N C T I O N    &    O P T I M I Z E R   ##########
## Construct cost function
cost_function = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(logits = pred, labels = y))
## Construct optimizer
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost_function)

# Evaluate model
correct_pred = tf.equal(tf.argmax(pred,1), tf.argmax(y,1))
accuracy = tf.reduce_mean(tf.cast(correct_pred, tf.float32))


#################  T R A I N    &    T E S T  ##################

## Initialize model
CNN_model = tf.global_variables_initializer()

with tf.Session() as session:
	session.run(CNN_model)
	step = 1
	#########  T R A I N  ##########
	while step * batch_size < training_iters:
		train_data,train_labels = mnist.train.next_batch(batch_size)
		session.run(optimizer, feed_dict ={x:train_data, y:train_labels, keep_prob: dropout })
		#avg_cost += session.run(cost_function,feed_dict={x:train_data, y:train_labels})/total_batches
		if step % display_step == 0:
			acc = session.run(accuracy, feed_dict={x: train_data,y: train_labels,keep_prob: 1.})
			# Calculate batch loss
			loss = session.run(cost_function, feed_dict={x: train_data,y: train_labels,keep_prob: 1.})
			print "Training iters " + str(step*batch_size) +", Minibatch Loss= " + "{:.6f}".format(loss) + ", Training Accuracy= " + "{:.5f}".format(acc)
		step+=1



	#########  T E S T ##########
	print "Optimization Finished!"
	# Calculate accuracy for 256 mnist test images
	print "Testing Accuracy:",session.run(accuracy,feed_dict={x: mnist.test.images[:256], y: mnist.test.labels[:256],keep_prob: 1.})
