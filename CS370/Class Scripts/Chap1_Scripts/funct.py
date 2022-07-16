#!/opt/rh/python33/root/usr/bin/python3

from math import atan
def finite_diff(f,x,h=0.0001):
  df=(f(x+h)-f(x-h))/(2.0*h)
  ddf=(f(x+h)-2.0*f(x)+f(x-h))/h**2
  return df,ddf

x = 0.5     # What about h?
df,ddf=finite_diff(atan,x)

print('First deriv. =',df)
print('Second deriv. =',ddf)


