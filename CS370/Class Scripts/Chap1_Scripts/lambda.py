#!/opt/rh/python33/root/usr/bin/python3

c = lambda x,y: x**2 + y **2 # define 2D function on the fly
print (c(3,4),'\n')

L = [lambda x : x**2, lambda x : x**3, lambda x : x**4] 
for f in L: print(f(3))
