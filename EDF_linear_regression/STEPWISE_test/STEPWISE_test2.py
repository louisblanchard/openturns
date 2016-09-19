#! /usr/bin/env python

import openturns as ot
from math import log

Sample = ot.NumericalSample.ImportFromTextFile("DATA_test2.csv", ",")
# Sample.setDescription(["BIO","SAL","pH","K","Na","Zn"])
print(Sample)

X = Sample[:, 1:4]
Y = Sample[:, 0]

# Build a model BIO~SAL+pH+K+Na+Zn
factory = ot.LinearModelStepwiseFactory(X, Y)
factory.add(factory.getInteractions(X.getDescription(), 1))

i_0 = factory.getIndices(["1"])
i_max = factory.getIndices()
i_min = i_0

## Forward
lm_forward_AIC_result = factory.buildForward(i_min, i_max, 2)
lm_forward_BIC_result = factory.buildForward(i_min, i_max, log(45))

## Backward
lm_backward_AIC_result = factory.buildBackward(i_max, i_min, 2)
lm_backward_BIC_result = factory.buildBackward(i_max, i_min, log(45))

## Both
lm_both_AIC_result = factory.buildBoth(i_min, i_max, i_0, 2)
lm_both_BIC_result = factory.buildBoth(i_min, i_max, i_0, log(45))


lm_forward_AIC_result.printANOVAtable()
lm_forward_BIC_result.printANOVAtable()

lm_backward_AIC_result.printANOVAtable()
lm_backward_BIC_result.printANOVAtable()

lm_both_AIC_result.printANOVAtable()
lm_both_BIC_result.printANOVAtable()

