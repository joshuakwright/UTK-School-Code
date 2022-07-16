#!/usr/bin/python3

class Robot:
    """Common base class for all robots"""
    robotCount = 0  # Shared amount all instances of this class

    def __init__ (self, name, color, weight):    
        self.name = name
        self.color = color
        self.weight = weight

    def introduce_self(self):
        print("My name is " + self.name)
        print("My color is " + self.color)
        print("My weight is ", self.weight)
        Robot.robotCount+=1

    def displayCount(self):
        print("Total Robot Count: ",  Robot.robotCount)

r1 = Robot("Tom", "silver", 30)
r1.introduce_self()
r1.displayCount()

print("\n")
r2 = Robot("Sam", "gold", 40)
r2.introduce_self()
r2.displayCount()

print("\nFinal robot count: %d" % Robot.robotCount)

print("\nRobot.__doc__:", Robot.__doc__)
print("Robot.__name__:", Robot.__name__)
print("Robot.__module__:", Robot.__module__)
print("Robot.__bases__:", Robot.__bases__)
print("Robot.__class__:", Robot.__class__)
print("Robot.__dict__:", Robot.__dict__)

