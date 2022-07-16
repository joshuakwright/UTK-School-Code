#!/opt/rh/python33/root/usr/bin/python3
## example2_3 on p. 42

import site, os
site.addsitedir('/home/mberry/courses/cs370/fall10/Progs_py')  # Append to path
from numpy import array
from gaussElimin import *

os.system('clear')  # This is handy for clearing the console screen

A = array([[ 4.0, -2.0,  1.0], \
           [-2.0,  4.0, -2.0], \
           [ 1.0, -2.0, 4.0]])
b=array([11.0, -16.0, 17.0])
A_old=A.copy()  # original contents of A will be overwritten
print ('rhs vector b=\n',b,'\n')
x=gaussElimin(A,b)
print ('original A =\n',A_old,'\n')
print ('current  A =\n',A,'\n')
print ('current vector b=\n',b,'\n')  # rhs vector overwritten with solution
print ('\nSolution vector x=', x)
print ('\n Is x a copy or alias of b? Execute x[0]+=1')
x[0]+=1
print ('\n x[0]=',x[0],' b[0]=',b[0])
input("\nPress return to exit")  
