#!/opt/rh/python33/root/usr/bin/python3

from numpy import *
a=array([[ 4.0,-2.0, 1.0],\
         [-2.0, 4.0,-2.0],\
         [ 1.0,-2.0, 3.0]])

b=array([1.0,4.0,2.0])

print('Dot product for b ->',dot(b,b)) # dot product

print('Matrix-vec prod ->', dot(a,b)) # matrix-vector product

print('Elementwise mult-> ',multiply(a,b)) 	# element-wise product

print('Principal diag ->  ',diagonal(a)) 		# principal diagonal

print('First sub-diag ->   ',diagonal(a,1))	# first subdiagonal

print('Trace -> ',trace(a)) 		# sum of diagonal elements

