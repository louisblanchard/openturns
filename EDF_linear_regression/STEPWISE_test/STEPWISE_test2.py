#! /usr/bin/env python

import openturns as ot
from math import log

Sample = ot.NumericalSample.ImportFromTextFile("DATA_test2.csv", ",")
# Sample.setDescription(["BIO","SAL","pH","K","Na","Zn"])
print(Sample)

X = Sample[:, 1:4]
Y = Sample[:, 0]


penality_BIC = log(X.getSize())
penality_AIC = 2.
maxiteration = 1000

# Build a model BIO~SAL+pH+K+Na+Zn
factory = ot.LinearModelStepwiseFactory(X.getDescription(),0,penality_BIC,maxiteration)
factory.add(factory.getInteractions(1))

i_0 = factory.getIndices(["1"])
i_min = i_0

## Forward
factory.setDirection(1)
factory.setPenalty(penality_AIC)
lm_forward_AIC_result = factory.build(X, Y, i_min)
factory.setPenalty(penality_BIC)
lm_forward_BIC_result = factory.build(X, Y, i_min)

## Backward
factory.setDirection(-1)
factory.setPenalty(penality_AIC)
lm_backward_AIC_result = factory.build(X, Y, i_min)
factory.setPenalty(penality_BIC)
lm_backward_BIC_result = factory.build(X, Y, i_min)

## Both
factory.setDirection(0)
factory.setPenalty(penality_AIC)
lm_both_AIC_result = factory.build(X, Y, i_min, i_0)
factory.setPenalty(penality_BIC)
lm_both_BIC_result = factory.build(X, Y, i_min, i_0)


lm_forward_AIC_result.printANOVAtable()
lm_forward_BIC_result.printANOVAtable()

lm_backward_AIC_result.printANOVAtable()
lm_backward_BIC_result.printANOVAtable()

lm_both_AIC_result.printANOVAtable()
lm_both_BIC_result.printANOVAtable()

