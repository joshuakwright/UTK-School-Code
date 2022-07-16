# Joshua Wright jwrig117
# Hwk 1 for COSC 370
# August 2013 High Temps for Knoxville,TN

import numpy as np
from matplotlib import pyplot as plt

xData = np.arange(1,32)
tData = [86, 87, 84, 86, 86, 86, 84, 83, 90, 89, 88, 85, 86, 79, 83, 81,
         75, 80, 81, 85, 81, 88, 89, 87, 84, 85, 86, 88, 88, 90, 90]
#array/ variable used to compute the rolling average
avg = []
cursum = []
num = 0

#populates cursum with rolling total temp
for x in tData:
    num = num + x
    cursum.append(num)

print('Day:', end=' ')
num = 1
#populates avg with rolling mean and prints each avg  and prints day number
for x in cursum:
    avg.append(round((x / num),2))
    print(num, end=' ')
    num += 1

print()
print("Avg:", end=' ')

#prints rolling average for each day
for x in avg:
    print(x, end=' ')

#graph title and axis labels
plt.title("High Temperatures for Knoxville, TN - August 2013")
plt.xlabel("Day")
plt.ylabel("High Temp")

#plots points
plt.scatter(xData,tData, color='red', marker='o', edgecolor='black', linewidth=.5)
#plots line connecting points
plt.plot(xData,tData, color='blue', linewidth=1)
#plots rolling avg line
plt.plot(xData,avg, linestyle='--', color='green', linewidth=1)
#range resize
plt.xlim(0, 32)
plt.ylim(70,95)
#grid lines
plt.grid(color='black', linestyle='--', linewidth=.5)
#montly avg text
plt.text(15, 86, "Monthly Avg", color="Green")
plt.show()
