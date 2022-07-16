#!/opt/rh/python33/root/usr/bin/python3
from numpy import *
a=zeros((3,3),dtype=float)
a[0]=[2.0,1.1,1.5]  # change row
a[1,1]=5.2			# change element
a[2,0:2]=[8.0,-3.3]	# change slice
print(a)
