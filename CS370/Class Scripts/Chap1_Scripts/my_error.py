#!/opt/rh/python33/root/usr/bin/python3
## module my_error
'''my_error(string).
   Prints ‘string’ and terminates.
''' 
import sys
def my_error(string) :
	print(string)
	input('Press return to exit')
	sys.exit()	
