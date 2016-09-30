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
#   X: input sample
#   Y: output sample
#   direction: +1 FORWARD, 0 BOTH, -1 BACKWARD
#   penalty: multiplier of number of degrees of freedom
#   maxiteration: maximum number of iterations
algo = ot.LinearModelStepwiseAlgorithm(X, Y, 0, penalty_BIC, maxiteration)
# (X1+X2+X3+X4)^3
algo.addInteractions(3)

# I(Xi)^2
algo.addPower(2)

# I(Xi)^3
algo.addPower(3)

# To add some user-defined columns, it is also possible to pass
# a NumericalSample, for instance:
#   expI = ot.NumericalSample(np.exp(X))
#   expI.setDescription(["exp(x1)", "exp(x2)", "exp(x3)", "exp(x4)"])
#   algo.add(expI)

i_min = algo.getIndices(algo.getInteractions(0))
algo.setMinimalIndices(i_min)

## Forward
algo.setDirection(1)
algo.setPenalty(penalty_AIC)
lm_forward_AIC_result = algo.getResult()
algo.setPenalty(penalty_BIC)
lm_forward_BIC_result = algo.getResult()

## Backward
algo.setDirection(-1)
algo.setPenalty(penalty_AIC)
lm_backward_AIC_result = algo.getResult()
algo.setPenalty(penalty_BIC)
lm_backward_BIC_result = algo.getResult()

## Both
algo.setDirection(0)
algo.setPenalty(penalty_AIC)
i_0 = algo.getIndices(algo.getInteractions(1))
algo.setStartIndices(i_0)
lm_both_AIC_result = algo.getResult()
algo.setPenalty(penalty_BIC)
lm_both_BIC_result = algo.getResult()

lm_forward_AIC_result.printANOVAtable()
lm_forward_BIC_result.printANOVAtable()

lm_backward_AIC_result.printANOVAtable()
lm_backward_BIC_result.printANOVAtable()

lm_both_AIC_result.printANOVAtable()
lm_both_BIC_result.printANOVAtable()
