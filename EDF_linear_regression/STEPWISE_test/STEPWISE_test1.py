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
factory = ot.LinearModelStepwiseFactory(X.getDescription(),0,penalty_BIC,maxiteration)
# (X1+X2+X3+X4)^3
factory.add(factory.getInteractions(3))

# I(Xi)^2
factory.add(factory.getPolynomial(2))

# I(Xi)^3
factory.add(factory.getPolynomial(3))

# To add some user-defined columns, it is also possible to pass
# a NumericalSample, for instance:
#   expI = ot.NumericalSample(np.exp(X))
#   expI.setDescription(["exp(x1)", "exp(x2)", "exp(x3)", "exp(x4)"])
#   factory.add(expI)

i_min = factory.getIndices(["1"])
i_0 = factory.getIndices(factory.getInteractions(1))

## Forward
factory.setDirection(1)
factory.setPenalty(penalty_AIC)
lm_forward_AIC_result = factory.build(X, Y, i_min)
factory.setPenalty(penalty_BIC)
lm_forward_BIC_result = factory.build(X, Y, i_min)

## Backward
factory.setDirection(-1)
factory.setPenalty(penalty_AIC)
lm_backward_AIC_result = factory.build(X, Y, i_min)
factory.setPenalty(penalty_BIC)
lm_backward_BIC_result = factory.build(X, Y, i_min)

## Both
factory.setDirection(0)
factory.setPenalty(penalty_AIC)
lm_both_AIC_result = factory.build(X, Y, i_min, i_0)
factory.setPenalty(penalty_BIC)
lm_both_BIC_result = factory.build(X, Y, i_min, i_0)

lm_forward_AIC_result.printANOVAtable()
lm_forward_BIC_result.printANOVAtable()

lm_backward_AIC_result.printANOVAtable()
lm_backward_BIC_result.printANOVAtable()

lm_both_AIC_result.printANOVAtable()
lm_both_BIC_result.printANOVAtable()
