#!/opt/rh/python33/root/usr/bin/python3

from numpy import *
a=array([[ 4.0,-2.0, 1.0],\
         [-2.0, 4.0,-2.0],\
         [ 1.0,-2.0, 3.0]])

b=array([1.0,4.0,2.0])

print('Index of largest element in b ->',argmax(b),'\n')
print(identity(3),'\n') # identity matrix 

c = a.copy()  # copy of array a (not an alias)
print(c)
