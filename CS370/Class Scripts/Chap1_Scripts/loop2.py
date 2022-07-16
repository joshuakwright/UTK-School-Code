#!/opt/rh/python33/root/usr/bin/python3
list=['a','b','c','d']
bound=len(list)
char_input=eval(input('Type a char: '))
for i in range(bound) :      # What does range return?
    if list[i] == char_input :
       print(char_input,' is in position ',i+1,' in list')
       break
    else:
       if i == bound-1:
          print(char_input,' is not in list')
#>>>Type a char: ‘a’
#>>>Type a char: ‘e’
