
import numpy as np
import matplotlib.pyplot as plt
from tensorflow.examples.tutorials.mnist import input_data
import os
import gzip
import struct

path = os.getcwd()


"""

### Read image from downloading MNIST data ###

mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)

pixels,real_values = mnist.train.next_batch(1)


print pixels
print real_values

image = np.reshape(pixels,[28,28])
plt.imshow(image)
plt.show()

"""

### Read image from downloaded MNIST data ###

filename = path + "/MNIST_data/train-images.idx3-ubyte"
img_file = open(filename,'r+b')

def read_image(file_name, idx_image):
	"""
	file_name: If used for the MNIST dataset, should be either 
					train-images-idx3-ubyte or t10k-images-idx3-ubyte
	idx_image: index of the image you want to read.
	"""
	img_file = open(file_name,'r+b')
	
	img_file.seek(0)
	magic_number = img_file.read(4)
	magic_number = struct.unpack('>i',magic_number)
		
	data_type = img_file.read(4)
	data_type = struct.unpack('>i',data_type)
	print('Number of Images: '+str(data_type[0]))

	
	dim = img_file.read(8)
	dimr = struct.unpack('>i',dim[0:4])
	dimr = dimr[0]
	
	dimc = struct.unpack('>i',dim[4:])
	dimc = dimc[0]

	image = np.ndarray(shape=(dimr,dimc))
	img_file.seek(16+dimc*dimr*idx_image)
	
	for row in range(dimr):
		for col in range(dimc):
			tmp_d = img_file.read(1)
			tmp_d = struct.unpack('>B',tmp_d)
			image[row,col] = tmp_d[0]
	
	img_file.close()
	return image

image = read_image(filename,1)
img_plot = plt.imshow(image)
plt.show()