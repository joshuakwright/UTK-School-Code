#!/opt/rh/python33/root/usr/bin/python3
#
# Demo of Bezier curves ...
#
import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches

verts = [
    (0., 0.),  # P0
    (0.2, 1.), # P1
    (1., 0.8), # P2
    (0.8, 0.), # P3
    ]

codes = [Path.MOVETO,
         Path.CURVE4,
         Path.CURVE4,
         Path.CURVE4,
         ]

path = Path(verts, codes)

fig = plt.figure()
ax = fig.add_subplot(111)
patch = patches.PathPatch(path, facecolor='none', lw=2)
ax.add_patch(patch)

xs, ys = zip(*verts)  # The zip() function returns an iterator
                      # of tuples based on the iterable objects.
ax.plot(xs, ys, 'x--', lw=2, color='black', ms=10)

ax.text(-0.05, -0.05, 'P0')
ax.text(0.15, 1.05, 'P1')
ax.text(1.05, 0.85, 'P2')
ax.text(0.85, -0.05, 'P3')

ax.set_xlim(-0.1, 1.1)
ax.set_ylim(-0.1, 1.1)
plt.show()

# Path Codes...
#
# Code 	Vertices     Description
#
# STOP 	        1 (ignored)  A marker for the end of the entire path (currently
#                            not required and ignored)
# MOVETO 	1 	     Pick up the pen and move to the given vertex.
# LINETO 	1 	     Draw a line from the current position to the given vertex.
# CURVE3 	2            (1 control point, 1 endpoint) 	Draw a quadratic
#                            Bezier curve from the current position, with the given 
#                            control point, to the given end point.
# CURVE4 	3 (2 control points, 1 endpoint) 
#                            Draw a cubic Bezier curve from the current position,
#                            with the given control points, to the given end point.
# CLOSEPOLY 	1 (point itself is ignored) 
#                            Draw a line segment to the start point of the current 
#                            polyline.
