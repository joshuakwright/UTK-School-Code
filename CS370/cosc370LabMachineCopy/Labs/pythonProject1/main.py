#
# August 2013 High Temps for Knoxville,TN
# Hwk 1 for COSC 370
#

import numpy as np
from matplotlib import pyplot as plt

xData = np.arange(1,32)
tData = [86, 87, 84, 86, 86, 86, 84, 83, 90, 89, 88, 85, 86, 79, 83, 81,
         75, 80, 81, 85, 81, 88, 89, 87, 84, 85, 86, 88, 88, 90, 90]
avg = []
cursum = []
num = 0

for x in tData:
    num = num + x
    cursum.append(num)

print('Day:', end=' ')
num = 1
for x in cursum:
    avg.append(round((x / num),2))
    print(num, end=' ')
    num += 1

print()
print("Avg:", end=' ')

for x in avg:
    print(x, end=' ')

plt.title("High Temperatures for Knoxville, TN - August 2013")
plt.xlabel("Day")
plt.ylabel("High Temp")

plt.scatter(xData,tData, color='red', marker='o', edgecolor='black', linewidth=.5)
plt.plot(xData,tData, color='blue', linewidth=1)
plt.plot(xData,avg, linestyle='--', color='green', linewidth=1)
plt.xlim(0, 32)
plt.ylim(70,95)
plt.grid(color='black', linestyle='--', linewidth=.5)
plt.text(15, 86, "Monthly Avg", color="Green")
plt.show()
