#! /usr/bin/env python

from openturns.viewer import View
import openturns as ot
import pandas as pd

# First column in this CSV file is country name, use
# pandas to easily filter it out.
data = pd.read_csv("LifeCycleSavings.csv", index_col=0)

Sample = ot.NumericalSample(data.values)
Sample.setName("LifeCycleSavings")
Sample.setDescription(["sr","pop15","pop75","dpi","ddpi"])

sr    = Sample[:,0]
pop15 = Sample[:,1]
pop75 = Sample[:,2]
dpi   = Sample[:,3]
ddpi  = Sample[:,4]

# model1
outputSample = Sample[:,0]
inputSample = Sample[:,1:5]

algo1 = ot.LinearModelAlgorithm(inputSample, outputSample)
result1 = algo1.getResult()

# for plot in ["drawResidualsVsFitted", "drawScaleLocation", "drawQQplot", "drawCookDistance", "drawResidualsVsLeverages", "drawCookVsLeverages"]:
#     graph = getattr(result1, plot)()
#     View(graph)

# plot of residuals versus fitted values
graph = result1.drawResidualsVsFitted()
View(graph)

# scale-location plot of sqrt(|residuals|) versus fitted values
graph = result1.drawScaleLocation()
View(graph)

# Normal quantiles-quantiles plot of standardized residuals
graph = result1.drawQQplot()
View(graph)

# plot of Cook's distances versus row labels
graph = result1.drawCookDistance()
View(graph)

# plot of residuals versus leverages that adds bands corresponding to Cook's distances of 0.5 and 1
graph = result1.drawResidualsVsLeverages()
View(graph)

# plot of Cook's distances versus leverage/(1-leverage)
graph = result1.drawCookVsLeverages()
View(graph)

# model2
f = ot.NumericalMathFunction('x','x^4','y')
outputSample = f(sr)
inputSample = pop75
inputSample.stack(dpi)

algo2 = ot.LinearModelAlgorithm(inputSample, outputSample)
result2 = algo2.getResult()

# plot of residuals versus fitted values
graph = result2.drawResidualsVsFitted()
View(graph)

# scale-location plot of sqrt(|residuals|) versus fitted values
graph = result2.drawScaleLocation()
View(graph)

# Normal quantiles-quantiles plot of standardized residuals
graph = result2.drawQQplot()
View(graph)

# plot of Cook's distances versus row labels
graph = result2.drawCookDistance()
View(graph)

# plot of residuals versus leverages that adds bands corresponding to Cook's distances of 0.5 and 1
graph = result2.drawResidualsVsLeverages()
View(graph)

# plot of Cook's distances versus leverage/(1-leverage)
graph = result2.drawCookVsLeverages()
View(graph)

