
import math
from sklearn.naive_bayes import MultinomialNB
from sklearn.naive_bayes import BernoulliNB

import numpy as np

#Create train data for MultinomialNB
Mul_d1 = [2, 2, 1, 0, 0, 0, 0, 0, 1]
Mul_d2 = [1, 1, 2, 1, 1, 0, 0, 0, 0]
Mul_d3 = [2, 1, 3, 5, 1, 1, 0, 2, 0]
Mul_d4 = [5, 1, 1, 4, 0, 0, 1, 1, 1]


Ber_d1 = [1, 1, 1, 0, 0, 0, 0, 0, 1]
Ber_d2 = [1, 1, 1, 1, 1, 0, 0, 0, 0]
Ber_d3 = [1, 1, 0, 1, 1, 0, 0, 1, 0]
Ber_d4 = [0, 0, 0, 1, 0, 0, 1, 1, 1]


trainDataMultinomial = np.array([Mul_d1,Mul_d2,Mul_d3,Mul_d4])
labelsMultinomial = np.array(['B','B','B','N'])

trainDataBernoulli = np.array([Ber_d1,Ber_d2,Ber_d3,Ber_d4])
labelsBernoulli  = np.array(['B','B','B','N'])

#test data using Multinomial
Mul_d5 = np.array([[2, 0, 0, 1, 0, 0, 0, 1, 0]])
Mul_d6 = np.array([[0, 1, 0, 0, 0, 0, 0, 1, 1]])
Mul_d7 = np.array([[0, 0, 0, 1, 4, 0, 5, 6, 4]])

Mul_testData = np.array((Mul_d5,Mul_d6,Mul_d7))

Ber_d5 = np.array([[1, 0, 0, 1, 0, 0, 0, 1, 0]])
Ber_d6 = np.array([[0, 1, 0, 0, 0, 0, 0, 1, 1]])
Ber_d7 = np.array([[1, 1, 0, 0, 0, 1, 1, 1, 1]])

Ber_testData = np.array((Ber_d5,Ber_d6,Ber_d7))

#Initialize MultinormialNB probability distribution
models_1 = MultinomialNB()
#Initialize Bernoulli probability distribution
models_2 = BernoulliNB()

#training using MultinormialNB
models_1.fit(trainDataMultinomial, labelsMultinomial)  
#training using Bernoulli
models_2.fit(trainDataBernoulli, labelsBernoulli)  

print 'Predict input data using MultinomialNB:'
for i in range(Mul_testData.shape[0]):
	print 'Predict for input data:' + str(Mul_testData[i])
	print 'Output label is: ' + models_1.predict(Mul_testData[i])[0]
	print 'Predict probability of two classes are: ' + str(models_1.predict_proba(Mul_testData[i]))
	print '\n'

print 'Predict input data using BernoulliNB:'
for i in range(Ber_testData.shape[0]):
	print 'Predict for input data:' + str(Ber_testData[i])
	print 'Output label is: ' + models_1.predict(Ber_testData[i])[0]
	print 'Predict probability of two classes are: ' + str(models_1.predict_proba(Ber_testData[i]))
	print '\n'