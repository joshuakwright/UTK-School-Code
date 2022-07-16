import numpy as np
import matplotlib.pyplot as plt
import time, random, threading
from collections import Counter

class Generator(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)
		self.frequencies = []
		plt.show()

	def run(self):
		while(True):
			newNumber = random.randint(1,10)
			self.frequencies.append(newNumber)
			dic = Counter(self.frequencies)
			listOfTuples = sorted(dic.items())
			plt.cla()
			plt.bar([ x [0] for x in listOfTuples ],[ x [1] for x in listOfTuples ])
			plt.draw()
			plt.pause(.1)

x = Generator()
x.start()
x.join()
