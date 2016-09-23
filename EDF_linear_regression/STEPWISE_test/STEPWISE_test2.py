#! /usr/bin/env python

import openturns as ot
from math import log

Sample = ot.NumericalSample.ImportFromTextFile("DATA_test2.csv", ",")
# Sample.setDescription(["BIO","SAL","pH","K","Na","Zn"])
print(Sample)

X = Sample[:, 1:4]
Y = Sample[:, 0]

# Build a model BIO~SAL+pH+K+Na+Zn
factory = ot.LinearModelStepwiseFactory(X.getDescription())
factory.add(factory.getInteractions(1))

i_0 = factory.getIndices(["1"])
i_min = i_0

## Forward
lm_forward_AIC_result = factory.build(X, Y, 1, i_min, [], 2)
lm_forward_BIC_result = factory.build(X, Y, 1, i_min, [], log(45))

## Backward
lm_backward_AIC_result = factory.build(X, Y, 2, i_min, [], 2)
lm_backward_BIC_result = factory.build(X, Y, 2, i_min, [], log(45))

## Both
lm_both_AIC_result = factory.build(X, Y, 3, i_min, i_0, 2)
lm_both_BIC_result = factory.build(X, Y, 3, i_min, i_0, log(45))


lm_forward_AIC_result.printANOVAtable()
lm_forward_BIC_result.printANOVAtable()

lm_backward_AIC_result.printANOVAtable()
lm_backward_BIC_result.printANOVAtable()

lm_both_AIC_result.printANOVAtable()
lm_both_BIC_result.printANOVAtable()

