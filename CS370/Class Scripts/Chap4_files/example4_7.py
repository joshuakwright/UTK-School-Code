#!/opt/rh/python33/root/usr/bin/python3
## example4_7 (pages 159-160)

from numpy import arange,zeros
import pylab

def f(x):  return x**4 - 6.4*x**3 + 6.45*x**2 + 20.538*x - 31.752
def df(x): return 4.0*x**3 - 19.2*x**2 + 12.9*x + 20.538

def newtonRaphson(x,tol=1.0e-9):  # simplified N-R method
    for i in range(30):     # run no more than 30 iterations
        dx = -2*f(x)/df(x)  # 2 is multiplicity of root sought
        x = x + dx          # (see page 161 in textbook)
        if abs(dx) < tol: return x,i
    print ('Too many iterations')

init = 2.0; root,numIter = newtonRaphson(init)
print ('\nInitial guess =',init)
print ('Root =',root)
print ('Number of iterations =',numIter)

init = 0.0; root,numIter = newtonRaphson(init)
print ('\nInitial guess =',init)
print ('Root =',root)
print ('Number of iterations =',numIter)

init = 3.0; root,numIter = newtonRaphson(init)
print ('\nInitial guess =',init)
print ('Root =',root)
print ('Number of iterations =',numIter)

print ('\nWait for figure containing plot of f(x) ...\n')

n=7
xData = arange(-2,n,0.75,dtype=float)
n2=xData.size
yData = zeros((n2),dtype=float)
for j in range(0,n2):
    yData[j]=f(xData[j])

pylab.xlabel("x")
my_title= 'Plot of f(x) for Example 4.7'
pylab.title(my_title)
pylab.ylabel("f(x)")
pylab.grid(True)
pylab.plot(xData, yData,'bo',xData,yData,'r-')
pylab.show()

input("Press return to exit")
