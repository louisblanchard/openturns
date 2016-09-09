#! /usr/bin/env python

import os
import traceback

try:

    # use non-interactive backend
    import matplotlib
    matplotlib.use('Agg')

    from openturns.viewer import View
    import openturns as ot

    X = ot.NumericalSample.ImportFromTextFile("DATA.csv", ",")
    print("X=",X) 


except:
    traceback.print_exc()
    os._exit(1)
