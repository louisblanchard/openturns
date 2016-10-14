#! /usr/bin/env python

import openturns as ot
from math import log

Sample = ot.NumericalSample.ImportFromTextFile("DATA_test1.csv", ",")
# Sample.setDescription(["R", "X1", "X2", "X3", "X4"])
print(Sample)

X = Sample[:, 1:5]
R = Sample[:, 0]

myLinearModel = ot.NumericalMathFunction(['x1', 'x2', 'x3', 'x4'], ['y'],
    ['14 - 7*x1 - 17*x2 - 7 *x3 - 3*x4 + 13*x3^2 - 16*x4^2 ' +
       ' - 4*x1^2 + 12*x1*x2 + 3*x2*x4 + 13*x3*x4 + 20*x1*x2*x3 ' +
       ' + 17*x1^3 - 10*x2^3 + 7*x4^3'])

Y = myLinearModel(X) + R
print(Y)

penalty_BIC = log(X.getSize())
penalty_AIC = 2.
maxiteration = 1000

# Build a model Y~(X1+X2+X3+X4)^3+I(Xi)^2+I(Xi)^3

#---------------- Forward / Backward------------------- 
#   X: input sample
#   basis : Basis
#   Y: output sample
#   i_min:  indices of minimal model
#   direction: Boolean (True FORWARD, False BACKWARD)
#   penalty: multiplier of number of degrees of freedom
#   maxiteration: maximum number of iterations

#---------------- Both------------------- 
#   X: input sample
#   basis : Basis
#   Y: output sample
#   i_min : indices of minimal model
#   i_0   : indices of start model
#   penalty: multiplier of number of degrees of freedom
#   maxiteration: maximum number of iterations

# todo : define basis 

# (X1+X2+X3+X4)^3
algo.addInteractions(3)

# I(Xi)^2
algo.addPower(2)

# I(Xi)^3
algo.addPower(3)

i_min = algo.getIndices(algo.getInteractions(0))

# To add some user-defined columns, it is also possible to pass
# a NumericalSample, for instance:
#   expI = ot.NumericalSample(np.exp(X))
#   expI.setDescription(["exp(x1)", "exp(x2)", "exp(x3)", "exp(x4)"])
#   algo.add(expI)

for k in [penalty_AIC, penalty_BIC]:
  ## Forward / Backward
  for forward in [True, False]
    algo = ot.LinearModelStepwiseAlgorithm(X, basis, Y, i_min, forward, k, maxiteration)
    algo_result = algo.getResult()
    algo_result.printANOVAtable()
  ## Both
  algo = ot.LinearModelStepwiseAlgorithm(X, basis, Y, i_min, i_0, k, maxiteration)
  algo_result = algo.getResult()
  algo_result.printANOVAtable()

