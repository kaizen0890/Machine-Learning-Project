import os
import math
from sklearn.naive_bayes import MultinomialNB
import numpy as np

dictNumber = 2500

path = (os.getcwd())
dataPath = path + "/Classify_Email_Data/"
trainDataFilename = path + "/Classify_Email_Data/train-features-50.txt"
trainLabelsFileName = path + "/Classify_Email_Data/train-labels-50.txt"
testDataFileName = path + "/Classify_Email_Data/test-features.txt"
testLabelsFileName = path + "/Classify_Email_Data/test-labels.txt"


def read_train_data():
	data = np.zeros((50, 2500), dtype = int)
	with open(trainDataFilename) as file_object:
		lines = file_object.readlines()
		for line in lines:
			data[int(line.split(' ')[0]) - 1,int(line.split(' ')[1]) - 1] = int(line.split(' ')[2])

	return data

def read_train_labels():
	data = np.zeros(50,dtype = int)
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

	return data
	

def read_test_labels():
	data = np.zeros(260,dtype = int)
	i = 0
	with open(testLabelsFileName) as file_object:
		lines = file_object.readlines()
		for line in lines:
			i +=1
			data[i-1] = int(line)
	return data
	

training_data = read_train_data()
training_labels = read_train_labels()


test_data = np.array([read_test_data()])
test_labels = read_test_labels()

#Initialize MultinormialNB probability distribution
models_1 = MultinomialNB()
#training using MultinormialNB
models_1.fit(training_data, training_labels)  

result = np.zeros((1,260))
print result
for i in range(260-1):
	result[i] = str(models_1.predict(test_data[i])[0])

