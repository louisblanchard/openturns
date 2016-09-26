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

# Build a model Y~(X1+X2+X3+X4)^3+I(Xi)^2+I(Xi)^3
factory = ot.LinearModelStepwiseFactory(X.getDescription())
# (X1+X2+X3+X4)^3
factory.add(factory.getInteractions(3))

# I(Xi)^2
factory.add(factory.getPolynomial(2))

# I(Xi)^3
factory.add(factory.getPolynomial(3))

i_min = factory.getIndices(["1"])
i_0 = factory.getIndices(factory.getInteractions(1))

## Forward
lm_forward_AIC_result = factory.build(X, Y, 1, i_min, [], 2)
lm_forward_BIC_result = factory.build(X, Y, 1, i_min, [], log(100))

## Backward
lm_backward_AIC_result = factory.build(X, Y, 2, i_min, [], 2)
lm_backward_BIC_result = factory.build(X, Y, 2, i_min, [], log(100))

## Both
lm_both_AIC_result = factory.build(X, Y, 3, i_min, i_0, 2)
lm_both_BIC_result = factory.build(X, Y, 3, i_min, i_0, log(100))

lm_forward_AIC_result.printANOVAtable()
lm_forward_BIC_result.printANOVAtable()

lm_backward_AIC_result.printANOVAtable()
lm_backward_BIC_result.printANOVAtable()

lm_both_AIC_result.printANOVAtable()
lm_both_BIC_result.printANOVAtable()
