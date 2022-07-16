#!/usr/bin/python3

class Point:
    """Common base class for all Points"""
    x = 0 
    y = 0

    def addPoint(self, pl):
        newpoint = Point()
        newpoint.x = self.x + p1.x
        newpoint.y = self.y + pl.y
        return newpoint

a = Point()
a.x = 20
print("\na.__doc__:", a.__doc__)
#print("a.__name__:", a.__name__)
print("a.__module__:", a.__module__)
#print("a.__bases__:", a.__bases__)
print("a.__class__:", a.__class__)
print("a.__dict__:", a.__dict__)
a.y = 20
print("a.__dict__:", a.__dict__)
