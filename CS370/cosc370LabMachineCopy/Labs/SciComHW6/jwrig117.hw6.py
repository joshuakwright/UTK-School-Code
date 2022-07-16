#!/usr/bin/env python3
#
# Template for Problem 10 from Problem Set 3.2
#
# Joshua Wright HW6
#
# Using the thermal efficiencies of progressively more efficient engines
# developed over past years, the principle of least-squares curve fitting
# is used to create a linear regression model of the efficiencies in order
# to predict the efficiencies of engines in the year 2000 and output what
# degree of polynomial equations will yield a viable answer

from numpy import array, zeros
from polyFit import *
from matplotlib import pylab


def evalPoly(c, x):  # c stores coefficients for polynomial
    m = len(c) - 1  # (copied from polyFit module)
    p = c[m]
    for j in range(m):
        p = p * x + c[m - j - 1]
    return p


xData = array([1718, 1767, 1774, 1775, 1792, 1816, 1828, 1834, 1878, 1906])  # years
yData = array([0.5, 0.8, 1.4, 2.7, 4.5, 7.5, 12.0, 17.0, 17.2, 23.0])  # efficiencies (as percentages)

minsdev = float("inf")
minpoly = 0
n = len(xData)
print('Degree  Stdev   2000P')
for m in range(1, 6):  # Try m=1,2,3,4,5 (degree of polynomial)
    ys = zeros((n), dtype='float')  # initialize y-coordinates for polynomial curve

    coeff = polyFit(xData, yData, m)  # get coefficients for n-th degree polynomial
    stdev = stdDev(coeff, xData, yData)  # get stdev of the error in the fit\
    proj = evalPoly(coeff, 2000) # evaluate the polynomial at year 2000

    #   Year 2000 projections >= 100 or < 0 are meaniningless
    if (stdev < minsdev) and proj < 100 and proj > 0:

        print('{:3d}\t{:5.3f}\t{:5.3f}\t{:s}'.format(m, stdev, proj, 'viable'))
        for j in range(0, n):
            ys[j] = evalPoly(coeff, xData[j])  # get y-coordinates of polynomial using x-coordinates in xData array

        pylab.figure()
        pylab.xlabel("x")
        pylab.ylabel("Thermal Efficiency")
        my_title = 'Fit with poly degree = ' + str(m) + '; green dot is 2000 projection'
        pylab.title(my_title)
        pylab.xlim(1710, 2015)
        pylab.ylim(-10, 50)
        pylab.scatter(2000, proj, color='green', marker='o', edgecolor='black')
        pylab.scatter(xData, yData, color='red', marker='o', edgecolor='black')
        pylab.plot(xData, ys, color='blue', linewidth=1)
        pylab.grid(color='black', linestyle='--', linewidth=.5)
        fname = 'degree' + str(m) + 'fit.png'
        pylab.savefig(fname)
    else:
        print('{:3d}\t{:5.3f}\t{:5.3f}\t{:s}'.format(m, stdev, proj, 'not viable'))
# --------------------------------------------------------------------------------------
# Table to stdout should look similar to this...
#
# Degree Stdev   2000P
#   1 	 2.855	 34.986	    viable
#   2	 2.768	 45.419	    viable
#   3	 2.266	 -6.602	    not viable
#   4	 2.234	 112.391.   not viable
#   5	 2.496	 113.726.   not viable
# --------------------------------------------------------------------------------------
