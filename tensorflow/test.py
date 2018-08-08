import tensorflow as tf
import numpy as np 
import matplotlib.image as mp_image
import matplotlib.pyplot as plt



matrix1 = np.array([(2,5,2),(2,0,2),(1,2,2)],dtype = 'float64')
matrix2 = np.array([(1,5,1),(1,4,1),(3,1,9)],dtype = 'float64')

ten_matrix1 = tf.constant(matrix1)
ten_matrix2 = tf.constant(matrix2)

## Calculate matrix transform
matrix_product = tf.matmul(matrix1, matrix2)
matrix_sum = tf.add(matrix1, matrix2)
matrix_det = tf.matrix_determinant(matrix_sum)
matrix_inverse = tf.matrix_inverse(matrix_sum)
matrix_indentity = tf.matmul(matrix_inverse,matrix_sum)


## Open image as tensor
filename = "tensor.png"
input_data = mp_image.imread(filename)
x = tf.Variable(input_data,name = 'x')
model = tf.global_variables_initializer()

## Calculate gradient of function

a = tf.placeholder(tf.float32)
b = 2*a**2
var_grad = tf.gradients(b,a)

## Create random number on tensorflow
norm = tf.random_normal([100], mean=0, stddev=2)
random_num = tf.random_uniform([50],minval=-1,maxval=1,dtype=tf.int32)

N = 5
u_init = np.zeros([N, N, N], dtype=np.float32)
for n in range(40):
	a,b = np.random.randint(0, N, 3)
	u_init[a,b,c] = np.random.uniform()


print u_init



with tf.Session() as sess:
	x = tf.transpose(x,perm = [1,0,2])
	sess.run(model)
	result = sess.run(x)


	print sess.run(var_grad, feed_dict = {a:1})
	print sess.run(norm)
	print sess.run(random_num)



#plt.imshow(result)
#plt.show()
	
	


