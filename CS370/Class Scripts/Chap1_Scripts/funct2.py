#!/opt/rh/python33/root/usr/bin/python3

def squares(a) :
  for i in range(len(a)):
     a[i]=a[i]**2

  return a

a = [1,2,3,4]
print(squares(a))
