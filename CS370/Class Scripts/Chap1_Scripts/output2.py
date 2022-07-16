#!/opt/rh/python33/root/usr/bin/python3

a = 123.456
n = 5789

print('n={:07d}'.format(n)) # padding with zeros
print('n={:7d}'.format(n))  # padding with blanks

print('{:11.3e}{:7d}'.format(a,n))
