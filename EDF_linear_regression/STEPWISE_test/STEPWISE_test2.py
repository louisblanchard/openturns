#! /usr/bin/env python

import openturns as ot
from math import log

Sample = ot.NumericalSample.ImportFromTextFile("DATA_test2.csv", ",")
# Sample.setDescription(["BIO","SAL","pH","K","Na","Zn"])
print(Sample)

X = Sample[:, 1:6]
Y = Sample[:, 0]

################################################################################################
# Build a model BIO~SAL+pH+K+Na+Zn
dim = X.getDimension()
enumerateFunction = ot.EnumerateFunction(dim)
factory = ot.OrthogonalProductPolynomialFactory([ot.MonomialFactory()]*dim, enumerateFunction)

# Build 'interactions' as a list of list [a1,a2,a3,a4,a5], and we will generate tensorized
# polynomials SAL^a1*pH^a2*K^a3*Na^a4*Zn^a5.

# BIO~SAL+pH+K+Na+Zn
interactions = []
interactions.append([0]*dim)
for i in xrange(dim):
  indices = [0]*dim
  indices[i] = 1
  # Y ~ I(Xi)^1
  interactions.append(indices[:])

basis = ot.Basis([factory.build(enumerateFunction.inverse(indices)) for indices in interactions])
################################################################################################

i_min = interactions.index([0,0,0,0,0])
i_0 = i_min

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

penalty_BIC = log(X.getSize())
penalty_AIC = 2.
maxiteration = 1000

for k in [penalty_AIC, penalty_BIC]:
  ## Forward / Backward
  for forward in [True, False]:
    algo = ot.LinearModelStepwiseAlgorithm(X, basis, Y, i_min, forward, k, maxiteration)
    algo_result = ot.LinearModelAnalysis(algo.getResult())
    algo_result.print()
  ## Both
  algo = ot.LinearModelStepwiseAlgorithm(X, basis, Y, i_min, i_0, k, maxiteration)
  algo_result = ot.LinearModelAnalysis(algo.getResult())
  algo_result.print()


