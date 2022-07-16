#!/opt/rh/python33/root/usr/bin/python3

x=[] # empty list
data = open('sunspots.txt','r')
for line in data :
	x.append(eval(line.split()[3]))
data.close()
print(x) # print intensities only (4th column)
