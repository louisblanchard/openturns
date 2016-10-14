#! /usr/bin/env python

import openturns as ot
from math import log

Sample = ot.NumericalSample.ImportFromTextFile("DATA_test2.csv", ",")
# Sample.setDescription(["BIO","SAL","pH","K","Na","Zn"])
print(Sample)

X = Sample[:, 1:6]
Y = Sample[:, 0]


penalty_BIC = log(X.getSize())
penalty_AIC = 2.
maxiteration = 1000

# Build a model BIO~SAL+pH+K+Na+Zn

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

algo.addInteractions(1)

i_0 = algo.getIndices(algo.getInteractions(0))
i_min = i_0


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


