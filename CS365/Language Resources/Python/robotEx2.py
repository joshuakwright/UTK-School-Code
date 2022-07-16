#!/usr/bin/python3

class Robot:
    def __init__ (self, name, color, weight):    
        self.name = name
        self.color = color
        self.weight = weight

    def introduce_self(self):
        print("My name is " + self.name)

r1 = Robot("Tom", "silver", 30)
r1.introduce_self()

r2 = Robot("Sam", "gold", 40)
r2.introduce_self()
