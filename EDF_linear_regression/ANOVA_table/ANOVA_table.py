#! /usr/bin/env python

import openturns as ot

Sample = ot.NumericalSample.ImportFromTextFile("DATA.csv", ",")
ctl = Sample[:,0]
trt = Sample[:,1]

inputSample = ot.NumericalSample(ctl.getSize(), [0])
inputSample.add(ot.NumericalSample(trt.getSize(), [1]))
inputSample.setDescription(["Trt"])

outputSample = ctl
outputSample.add(trt)
outputSample.setDescription(["weight"])

LinearModelAlgorithm algo(inputSample, outputSample)
result = algo.getResult()
result.printANOVAtable()
