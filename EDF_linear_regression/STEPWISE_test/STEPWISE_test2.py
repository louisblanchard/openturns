#! /usr/bin/env python

import os
import traceback

try:

    # use non-interactive backend
    import matplotlib
    matplotlib.use('Agg')

    from openturns.viewer import View
    import openturns as ot

    Sample = ot.NumericalSample.ImportFromTextFile("DATA_test2.csv", ",")
    BIO = Sample[:,0]
    SAL = Sample[:,1]
    pH  = Sample[:,2]
    K   = Sample[:,3]
    Na  = Sample[:,4]
    Zn  = Sample[:,5]
    N   = Sample.getSize()

    BIO.setName("BIO")
    SAL.setName("SAL")
    pH.setName("pH")
    K.setName("K")
    Na.setName("Na")
    Zn.setName("Zn")

except:
    traceback.print_exc()
    os._exit(1)
