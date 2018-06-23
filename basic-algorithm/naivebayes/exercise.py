import os
import math
from sklearn.naive_bayes import MultinomialNB
import numpy as np

dictNumber = 2500

path = (os.getcwd())
dataPath = path + "/Classify_Email_Data/"
testDataFileName = path + "/Classify_Email_Data/test-features.txt"
testLabelsFileName = path + "/Classify_Email_Data/test-labels.txt"

test = path + "/test.txt"


def read_train_data(number_training):
	data = np.zeros((number_training, 2500), dtype = int)
	if (number_training == 700):
		trainDataFilename = path + "/Classify_Email_Data/train-features.txt"
		trainLabelsFileName = path + "/Classify_Email_Data/train-labels.txt"
	if (number_training == 50) or (number_training == 100) or (number_training == 400):
		trainDataFilename = path + "/Classify_Email_Data/train-features-" + str(number_training) + ".txt"
		trainLabelsFileName = path + "/Classify_Email_Data/train-labels-" + str(number_training) + ".txt"

	with open(trainDataFilename) as file_object:
		lines = file_object.readlines()
		for line in lines:
			data[int(line.split(' ')[0]) - 1,int(line.split(' ')[1]) - 1] = int(line.split(' ')[2])

	return data

def read_train_labels(number_training):
	data = np.zeros(number_training,dtype = int)
	if (number_training == 700):
		trainDataFilename = path + "/Classify_Email_Data/train-features.txt"
		trainLabelsFileName = path + "/Classify_Email_Data/train-labels.txt"
	if (number_training == 50) or (number_training == 100) or (number_training == 400):
		trainDataFilename = path + "/Classify_Email_Data/train-features-" + str(number_training) + ".txt"
		trainLabelsFileName = path + "/Classify_Email_Data/train-labels-" + str(number_training) + ".txt"
		
	i = 0
	with open(trainLabelsFileName) as file_object:
		lines = file_object.readlines()
		for line in lines:
			i +=1
			data[i-1] = int(line)
	return data

def read_test_data():
	data = np.zeros((260, 2500), dtype = int)
	with open(testDataFileName) as file_object:
		lines = file_object.readlines()
		for line in lines:
			data[int(line.split(' ')[0]) - 1,int(line.split(' ')[1]) - 1] = int(line.split(' ')[2])
	testData = []
	for i in range(data.shape[0]):
		temp = np.array([data[i,:]])
		testData.append(temp)

	return testData


def read_test_labels():
	data = np.zeros(260,dtype = int)
	i = 0
	with open(testLabelsFileName) as file_object:
		lines = file_object.readlines()
		for line in lines:
			i +=1
			data[i-1] = int(line)
	return data
	

def naiveBayes_classifier():
	active  = True
	print "Welcome to Naive Bayes Classifer program!"
	while active:
		print "Please input number of training data (50/100/400/700)"
		temp = raw_input()
		
		if temp == '50' or temp == '100' or temp == '400' or temp == '700':
			number_training = int(temp)
			training_data = read_train_data(number_training)
			training_labels = read_train_labels(number_training)
			test_data = read_test_data()
			test_labels = read_test_labels()

			#Initialize MultinormialNB probability distribution
			models_1 = MultinomialNB()
			#training using MultinormialNB

			models_1.fit(training_data, training_labels) 

			result = np.zeros(260, dtype = int)
			for i in range(260):
				result[i] = str(models_1.predict(test_data[i])[0])

			count = 0
			for i in range(test_labels.shape[0]):
				if result[i] == test_labels[i]:
					count +=1
			rate = (float(count)/260.0)*100
			print "Classifying rate for " + str(number_training) + " training data is :" + str (rate) + " %"

			print "Do you want to continue? (y/n)"
			answer = raw_input()
			if answer == 'n' or answer == 'no':
				active = False
			if answer == 'y' or answer == 'yes':
				continue
		else:
			print "Your input number is wrong"
			print "Please try again!"



naiveBayes_classifier()




