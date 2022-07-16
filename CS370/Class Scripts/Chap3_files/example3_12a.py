#!/opt/rh/python33/root/usr/bin/python3
## example3_12 modified with an outlier data point
from numpy import array,zeros
from polyFit import *
import pylab

# 11 original data points (with a change in one of them) 

xData = array([-0.04,0.93,1.95,2.90,3.83,5.0,      \
                5.98,7.05,8.21,9.08,10.09])

yData = array([-8.66,-6.44,-4.36,-3.27,-0.88,0.87, \
                3.31,4.63,-6.19,7.4,8.85])

# Original data point (8.21, 6.19) changed to (8.21,-6.19)

while 1:
    try:
        n=len(xData)
        ys=zeros((n),dtype='float64')
        m = eval(input("\nDegree of polynomial ==>  (-1 to stop) "))
        if (m == -1) : break
        coeff = polyFit(xData,yData,m)

        print ("Coefficients are:\n",coeff)
        print ("Std. deviation =",stdDev(coeff,xData,yData))

        for j in range(0,n):  # Need to evaluate poly at each
                              # x-coordinate; store in ys[]
           for i in range(m+1):
             ys[j]+=coeff[i]*xData[j]**i

        pylab.xlabel("x")
        my_title= 'Fit with degree = ' + str(m)
        pylab.text(2,7,r'Example 3.12, pp. 140-141')
        pylab.title(my_title)
        pylab.ylabel("f(x)")
        pylab.plot(xData, ys, 'b') 
        pylab.plot(xData, yData, 'ro') # original data given in table
        pylab.show()

    except SyntaxError: break
input("\nFinished. Press return to exit")
