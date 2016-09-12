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
factory = ot.LinearModelStepwiseFactory(X, Y)
# (X1+X2+X3+X4)^3
factory.add(factory.getInteractions(X.getDescription(), 3))

# I(Xi)^2
for x in X.getDescription():
  factory.add(factory.getPolynomial(x, 2))

# I(Xi)^3
for x in X.getDescription():
  factory.add(factory.getPolynomial(x, 3))

i_0 = factory.getIndices(["1"])
i_max = factory.getIndices()
i_min = factory.getIndices(factory.getInteractions(X.getDescription(), 1))

## Forward
lm_forward_AIC_result = factory.build(i_min, i_max, i_min, 2)
lm_forward_BIC_result = factory.build(i_min, i_max, i_min, log(100))

## Backward
lm_backward_AIC_result = factory.build(i_max, i_min, i_max, 2)
lm_backward_BIC_result = factory.build(i_max, i_min, i_max, log(100))

## Both
lm_both_AIC_result = factory.build(i_min, i_max, i_0, 2)
lm_both_BIC_result = factory.build(i_min, i_max, i_0, log(100))

lm_forward_AIC_result.printANOVAtable()
lm_forward_BIC_result.printANOVAtable()

lm_backward_AIC_result.printANOVAtable()
lm_backward_BIC_result.printANOVAtable()

lm_both_AIC_result.printANOVAtable()
lm_both_BIC_result.printANOVAtable()
