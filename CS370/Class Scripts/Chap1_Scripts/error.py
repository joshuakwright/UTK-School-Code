#!/opt/rh/python33/root/usr/bin/python3

b=2
a = eval(input('Input a: '))  # What happens w/o eval function ?

try :
    c=b/a
    print('Divide succeeded')

except ZeroDivisionError :
    print('Divide by zero')
