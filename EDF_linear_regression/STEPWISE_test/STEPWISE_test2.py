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
#   X: input sample
#   Y: output sample
#   direction: +1 FORWARD, 0 BOTH, -1 BACKWARD
#   penalty: multiplier of number of degrees of freedom
#   maxiteration: maximum number of iterations
algo = ot.LinearModelStepwiseAlgorithm(X, Y, 0, penalty_BIC, maxiteration)
algo.addInteractions(1)

i_0 = algo.getIndices(algo.getInteractions(0))
i_min = i_0
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
algo.setStartIndices(i_0)
algo.setDirection(0)
algo.setPenalty(penalty_AIC)
lm_both_AIC_result = algo.getResult()
algo.setPenalty(penalty_BIC)
lm_both_BIC_result = algo.getResult()


lm_forward_AIC_result.printANOVAtable()
lm_forward_BIC_result.printANOVAtable()

lm_backward_AIC_result.printANOVAtable()
lm_backward_BIC_result.printANOVAtable()

lm_both_AIC_result.printANOVAtable()
lm_both_BIC_result.printANOVAtable()

