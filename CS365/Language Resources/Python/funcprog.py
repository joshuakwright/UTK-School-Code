#return type function
def add_r(a, b):
  """ return type function to find sum """
  x = a + b
  return x

#non return type function
def add_n(a, b):
  """ non return type function to return sum """
  x = a + b
  print (x)

#call the return type function and print
print (add_r(2,3))

#call the non return type function
add_n(2,3)



def func_empty(args):
   pass

def func_return():
    a = 10
    return a

def no_return():
    a = 10

print(func_return())
print(no_return())

def square(n):
    return n*n

print(square(4))
x = 4
x = square(x);
print(x)


x = "global"

def foo():
    global x  # will get an error if comment
    x = x * 2  
    print(x)

foo()

def foo2():
    x = 20

    def bar():
        global x
        x = 25
    
    print("Before calling bar: ", x)
    print("Calling bar now")
    bar()
    print("After calling bar: ", x)

foo2()
print("x in main: ", x)



# Show the use of lambda functions
double = lambda x: x * 2

print(double(5))
