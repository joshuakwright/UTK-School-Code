#!/usr/bin/python3

class Robot:
    
    def introduce_self(self):
        print("My name is " + self.name)

r1 = Robot()
r1.name = "Tom"
r1.color = "silver"
r1.weight = 30

r1.introduce_self()

r2 = Robot()
r2.name = "Sam"
r2.color = "gold"
r2.weight = 40

r2.introduce_self()
