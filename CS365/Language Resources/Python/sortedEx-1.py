#!/usr/bin/python3
from operator import itemgetter

# 3 ways to sort a dictionary by value in python
mydict = {'abcd':4, 'abc':3, 'ab':2, 'a':1}

# using sorted function along with Python operator.itemgetter() method
print("Dictionary: ", mydict)
sort_dict = dict(sorted(mydict.items(), key=itemgetter(1)))
print("Sorted Dictionary by value: ", sort_dict)

# using sorted funtion along with lambda function
print("Dictionary: ", mydict)
sort_dict = dict(sorted(mydict.items(), key=lambda item:item[1]))
print("Sorted Dictionary by value: ", sort_dict)

# using sorted function along with mydict.items()
print("Dictionary: ", mydict)
sort_dict = dict(sorted((value, key) for (key, value) in mydict.items()))
print("Sorted Dictionary by value: ", sort_dict)
