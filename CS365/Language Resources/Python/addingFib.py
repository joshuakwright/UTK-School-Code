import matplotlib.pyplot as plt

def nextFib(y):
	newFib = y[len(y)-1]+y[len(y)-2]
	return newFib

fig = plt.figure() 
#Row|Coloumn|Index
ax = fig.add_subplot(121)
x = [1,2,3,4,5]
y = [1,1,2,3,5]
ay = fig.add_subplot(122)
ax.plot(x, y);
ay.plot(y, x);

for _ in range(50):
	y.append(nextFib(y))
	x.append(len(x))
	plt.cla()
	ax.plot(x, y);
	ay.plot(y, x);
	plt.draw()
	plt.pause(.1)

plt.show()
