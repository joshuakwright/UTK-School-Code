#!/opt/rh/python33/root/usr/bin/python3
nMax=4
n=1
a=[] # empty list
while n < nMax:
      a.append(1.0/n)
      n=n+1
print(a)

a=[] # empty list
#for n in range(1,nMax):
for n in range(nMax):
      a.append(1.0/(n+1))
print(a)
