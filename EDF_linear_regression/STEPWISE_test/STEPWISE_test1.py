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

################################################################################################
# Build a model Y~(X1+X2+X3+X4)^3+I(Xi)^2+I(Xi)^3
dim = X.getDimension()
enumerateFunction = ot.EnumerateFunction(dim)
factory = ot.OrthogonalProductPolynomialFactory([ot.MonomialFactory()]*dim, enumerateFunction)

# Build 'interactions' as a list of list [a1,a2,a3,a4], and we will generate tensorized
# polynomials x1^a1*x2^a2*x3^a3*x4^a4.

# Y ~ (X1+X2+X3+X4)^4
interactions = [x for x in ot.Tuples([2]*dim).generate()]
# Remove X1*X2*X3*X4 to obtain Y ~ (X1+X2+X3+X4)^3
interactions.pop(interactions.index([1]*dim))
for i in xrange(dim):
  indices = [0]*dim
  indices[i] = 2
  # Y ~ I(Xi)^2
  interactions.append(indices[:])
  # Y ~ I(Xi)^3
  indices[i] = 3
  interactions.append(indices[:])

basis = ot.Basis([factory.build(enumerateFunction.inverse(indices)) for indices in interactions])
################################################################################################

i_min = [interactions.index([0,0,0,0])]
i_0 = i_min[:]
for i in xrange(dim):
  indices = [0]*dim
  indices[i] = 1
  i_0.append(interactions.index(indices))

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

ot.Log.Show(ot.Log.ALL)

algo = ot.LinearModelStepwiseAlgorithm(X, basis, Y, i_min, True, penalty_AIC, maxiteration)
algo.run()
algo_result = algo.getResult()
analysis = ot.LinearModelAnalysis(algo_result)

from openturns.viewer import View
graph = analysis.drawResidualsVsFitted()
graph.draw("xxx", 640, 480);

v = View(graph)
v.show()

# for k in [penalty_AIC, penalty_BIC]:
#   ## Forward / Backward
#   for forward in [True, False]:
#     algo = ot.LinearModelStepwiseAlgorithm(X, basis, Y, i_min, forward, k, maxiteration)
#     algo_result = ot.LinearModelAnalysis(algo.getResult())
#     algo_result.print()
#   ## Both
#   algo = ot.LinearModelStepwiseAlgorithm(X, basis, Y, i_min, i_0, k, maxiteration)
#   algo_result = ot.LinearModelAnalysis(algo.getResult())
#   algo_result.print()

