import os
import math
import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial.distance import cdist
import random

np.random.seed(7)

means = [[8,2],[2,10], [10,12]]
cov = [[1,0],[0,1]]

N = 500

X0 = np.random.multivariate_normal(means[0], cov, N)
X1 = np.random.multivariate_normal(means[1], cov, N)
X2 = np.random.multivariate_normal(means[2], cov, N)

X = np.concatenate((X0, X1, X2), axis = 0)


def rand_init_centroids(X):
	# randomly pick k rows of X as initial centroids
	return X[np.random.choice(X.shape[0], 3, replace=False)]

def calculate_distance(X, centroids):
	dist = cdist(X, centroids)
	# return index of the closest centroid
	return np.argmin(dist, axis = 1)

def update_centroids(X,labels):
	centroids = np.zeros((3, X.shape[1]))
	for k in range(3):
		# collect all points that are assigned to the k-th cluster
		Xk = X[labels == k, :]
		centroids[k,:] = np.mean(Xk, axis = 0) # then take average
	return centroids

def stopping_condition(centroids, new_centroids):
	return (set([tuple(a) for a in centroids]) == set([tuple(a) for a in new_centroids]))


def display(centroids,X, labels):
	K = np.amax(labels) + 1
	X0 = X[labels == 0, :]
	X1 = X[labels == 1, :]
	X2 = X[labels == 2, :]
	plt.plot(X0[:, 0], X0[:, 1], 'b^', markersize = 4, alpha = .8)
	plt.plot(X1[:, 0], X1[:, 1], 'go', markersize = 4, alpha = .8)
	plt.plot(X2[:, 0], X2[:, 1], 'rs', markersize = 4, alpha = .8)

	plt.axis('equal')
	plt.plot()
	plt.show()


def kmean(X):
	centroids = [rand_init_centroids(X)]
	labels = []
	it = 0

	while True:
		labels.append(calculate_distance(X, centroids[-1]))
		new_centroids = update_centroids(X,labels[-1])
		if stopping_condition(centroids[-1], new_centroids):
			break
		centroids.append(new_centroids)
		it += 1
	return (centroids, labels, it)


### Main


(centroids, labels, it) = kmean(X)
print centroids[-1]
display(centroids,X, labels[-1])




"""

x = range(-20,20)
y = range(-20,20)
fig = plt.figure()
ax1 = fig.add_subplot(111)

ax1.scatter(centroids[0,0], centroids[0,1], s=50, c='b', marker="s", label='first')
ax1.scatter(centroids[1,0], centroids[1,1], s=50, c='g', marker="s", label='second')
ax1.scatter(centroids[2,0], centroids[2,1], s=50, c='r', marker="s", label='third')
ax1.scatter(X[:,0], X[:,1], s=10, c='b', marker="s", label='first')

plt.show()

"""