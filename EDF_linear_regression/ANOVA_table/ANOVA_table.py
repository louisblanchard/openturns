#! /usr/bin/env python

import os
import traceback

try:

    # use non-interactive backend
    import matplotlib
    matplotlib.use('Agg')

    from openturns.viewer import View
    import openturns as ot

    Sample = ot.NumericalSample.ImportFromTextFile("DATA.csv", ",")
    ctl = Sample[:,0]
    trt = Sample[:,1]
    N   = Sample.getSize()
    ctl.setName("ctl")
    trt.setName("trt")

except:
    traceback.print_exc()
    os._exit(1)
