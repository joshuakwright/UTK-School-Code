# Joshua Wright jwrig117 Cosc370 Hw4

# !/usr/bin/env python3
# prob2-3-19 (hwk #4)
#
#  Problem 2.3.19 (pg. 101)
#
#  1) Implement the Ax(v) function below so that it will initialize
#     and return the matrix-vector product Ax, where A is given at
#     the top of page 101 in the textbook.
#
#  2) Use the provided conjGrad.py module to solve the system described
#     in problem #19 (uncomment and fill in the '??' lines and provide
#     the requested loop and final error difference).
#
# The correct solution is:
#
# [ 21.42857143 38.39285714  57.14285714   47.32142857   75.
#   90.17857143 92.85714286 124.10714286 128.57142857]
#
########################################################################

from numpy import zeros, array, linalg

from conjGrad import *


def Av(v):
    Ax = zeros((9)) * 1.0

    # creates A matrix(by row) and multiplies it by vector v
    # I know this seems a little overengineered but I wrote it this way because
    # if you replaced all the 9s with a variable, you could then create a matrix of
    # any size that followed the form of the one provided
    for i in range(0, 9):
        if (i - 3) >= 0:
            Ax[i] += 1.0 * v[i - 3]
        if ((i - 1) >= 0) and ((i) % 3 != 0):
            Ax[i] += 1.0 * v[i - 1]
        Ax[i] += -4.0 * v[i]
        if ((i + 1) < 9) and ((i + 1) % 3 != 0):
            Ax[i] += 1.0 * v[i + 1]
        if (i + 3) < 9:
            Ax[i] += 1.0 * v[i + 3]

    return Ax


b = array([0, 0, 100, 0, 0, 100, 200, 200, 300]) * (-1.0)
x = zeros((9)) * 1.0
tol = 1e-06
s1, numIter = conjGrad(Av, x, b, tol)
print("\nThe solution is:\n", s1)
print("\nNumber of iterations =", numIter, "using Tol: ", 1e-06)

print("\n CG Convergence Test")
print("Iterations   Tolerance")

tolerances = [1e-02, 1e-04, 1e-06, 1e-08, 1e-10, 1e-12, 1e-14, 1e-16]

# output the amount of iterations needed for each level of tolerance listed about and then outputs
# the difference in error between least and most tolerant
for tol in tolerances:
    x = zeros((9)) * 1.0
    s2, numIter = conjGrad(Av, x, b, tol)
    print('{:6d}      {:8.1e}'.format(numIter, tol))
print("\nError between vectors obtained with tol=1e-06 and tol=1e-16: ", linalg.norm(s2 - s1), "\n")
