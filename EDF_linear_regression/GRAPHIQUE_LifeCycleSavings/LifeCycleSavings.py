#! /usr/bin/env python

import os
import traceback

try:

    # use non-interactive backend
    import matplotlib
    matplotlib.use('Agg')

    from openturns.viewer import View
    import openturns as ot


    Sample = ot.NumericalSample.ImportFromTextFile("LifeCycleSavings_mod.csv", ",")
    Sample.setName("LifeCycleSavings")
    N = Sample.getSize()
    sr    = Sample[:,0]
    pop15 = Sample[:,1]
    pop75 = Sample[:,2]
    dpi   = Sample[:,3]
    ddpi  = Sample[:,4]

    # model1
    Sample1 = sr
    Sample2 = pop15 + pop75 + dpi + ddpi
    
    # model2
    f = ot.NumericalMathFunction('x','x^4','y')
    Sample1 = f(sr)
    Sample2 = pop75 + dpi

except:
    traceback.print_exc()
    os._exit(1)
