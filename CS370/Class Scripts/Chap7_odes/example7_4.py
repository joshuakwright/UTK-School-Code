#!/opt/rh/python33/root/usr/bin/python3
## example7_4
from numpy import array,zeros,float64
from printSoln import *
from run_kut4 import *

def F(x,y):
    F = zeros((2),dtype=float64)
    F[0] = y[1]
    F[1] = -0.1*y[1] - x
    return F

x = 0.0                 # Start of integration
xStop = 2.0             # End of integration
y = array([0.0, 1.0])   # Initial values of {y}
h = 0.25                # Step size
freq = 1                # Printout frequency

X,Y = integrate(F,x,y,xStop,h)
printSoln(X,Y,freq)
input("Press return to exit")
