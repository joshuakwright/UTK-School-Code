#!/opt/rh/python33/root/usr/bin/python3
def divide1() :
	    c=a/b 
	    print('a/b =',c,'\n')

def divide2() :
    c=a/b

a=100.0
b=5.0
print('Calling divide1 function...\n')
divide1()
print('Now calling divide2 function...\n')
divide2()
print('a/b =',c)
