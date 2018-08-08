
import tensorflow as tf 
import numpy as np  
import matplotlib.pyplot as plt
from tensorflow.examples.tutorials.mnist import input_data

mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)

## Get train image and train labels by batchs 
train_data,train_labels = mnist.train.next_batch(100)s
test_data, test_labels = mnist.test.next_batch(10)

## Convert data to tensor form
train_tensor = tf.placeholder("float", [None, 784])
test_tensor = tf.placeholder("float" , [784])

## Calculate distance between test tensor and train tensor
distance = tf.reduce_sum(tf.abs(tf.add(train_tensor,tf.negative(test_tensor))),reduction_indices=1)

## Predict label of test data
predict = tf.argmin(distance,0)

## Initialize model
model = tf.global_variables_initializer()

classify_rate = 0

with tf.Session() as session:
	session.run(model)
	for i in range(len(test_labels)):
		nn_index = session.run(predict, feed_dict = {train_tensor:train_data, test_tensor:test_data[i,:]})
		pred_label = np.argmax(train_labels[nn_index])
		true_label = np.argmax(test_labels[i])
		print "Test ", i , ":" , "Predicted label: ",pred_label, " | ", "True label: ", true_label
		if (pred_label == true_label):
			classify_rate += 1.0/len(test_labels)
	print "Classify rate is: ", classify_rate*100 , " %"








