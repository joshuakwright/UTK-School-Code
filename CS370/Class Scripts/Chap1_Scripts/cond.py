#!/opt/rh/python33/root/usr/bin/python3
def sign_of_a(a) :
  if a < 0.0 :
     sign = 'neg'
  elif a > 0.0 :
     sign = 'pos'
  else:
     sign = 'zero'
  return sign
a=2.0
print('a is ' + sign_of_a(a))
