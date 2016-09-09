#! /usr/bin/env python

import os
import traceback

try:

    # use non-interactive backend
    import matplotlib
    matplotlib.use('Agg')

    from openturns.viewer import View
    import openturns as ot

    Sample = ot.NumericalSample.ImportFromTextFile("DATA_test1.csv", ",")
    R  = Sample[:,0]
    X1 = Sample[:,1]
    X2 = Sample[:,2]
    X3 = Sample[:,3]
    X4 = Sample[:,4]
    R.setName("R")
    X1.setName("X1")
    X2.setName("X2")
    X3.setName("X3")
    X4.setName("X4")
    N = Sample.getSize()

except:
    traceback.print_exc()
    os._exit(1)
