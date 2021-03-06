#! /usr/bin/env python

from __future__ import print_function
import openturns as ot


def test_model(myModel):

    print('myModel = ',  myModel)

    spatialDimension = myModel.getSpatialDimension()
    dimension = myModel.getDimension()
    active = myModel.getActiveParameter()
    print('active=', active)
    print('parameter=', myModel.getParameter())
    print('parameterDescription=', myModel.getParameterDescription())



    x1 = ot.NumericalPoint(spatialDimension)
    x2 = ot.NumericalPoint(spatialDimension)
    for j in range(spatialDimension):
        x1[j] = -1.0 - j
        x2[j] = 3.0 + 2.0 * j

    eps = 1e-5
    if (dimension == 1):
        print('myModel(', x1, ', ', x2, ')=',  repr(myModel(x1, x2)))

        grad = myModel.partialGradient(x1, x2)
        print('dCov =', repr(grad))
        gradfd = ot.NumericalPoint(spatialDimension)
        for j in range(spatialDimension):
            x1_d = ot.NumericalPoint(x1)
            x1_d[j] = x1_d[j] + eps
            gradfd[j] = (myModel(x1_d, x2)[0, 0] - myModel(x1, x2)[0, 0]) / eps
        print('dCov (FD)=', repr(gradfd))
    else:
        print('myModel(', x1, ', ', x2, ')=',  repr(myModel(x1, x2)))

        grad = myModel.partialGradient(x1, x2)
        print('dCov =', repr(grad))

        gradfd = ot.Matrix(spatialDimension, dimension * dimension)
        covarianceX1X2 = myModel(x1, x2)
        # Symmetrize matrix
        covarianceX1X2.getImplementation().symmetrize()
        centralValue = ot.NumericalPoint(covarianceX1X2.getImplementation())
        # Loop over the shifted points
        for i in range(spatialDimension):
            currentPoint = ot.NumericalPoint(x1)
            currentPoint[i] += eps
            localCovariance = myModel(currentPoint, x2)
            localCovariance.getImplementation().symmetrize()
            currentValue = ot.NumericalPoint(
                localCovariance.getImplementation())
            for j in range(currentValue.getSize()):
                gradfd[i, j] = (currentValue[j] - centralValue[j]) / eps
        print('dCov (FD)=', repr(gradfd))

spatialDimension = 2


myDefautModel = ot.SquaredExponential([2.0], [3.0])
print('myDefautModel = ',  myDefautModel)
test_model(myDefautModel)

myModel = ot.SquaredExponential([2.0] * spatialDimension, [3.0])
test_model(myModel)


myDefautModel = ot.GeneralizedExponential([2.0], [3.0], 1.5)
print('myDefautModel = ',  myDefautModel)
test_model(myDefautModel)

myModel = ot.GeneralizedExponential([2.0] * spatialDimension, [3.0], 1.5)
test_model(myModel)


myDefautModel = ot.AbsoluteExponential([2.0], [3.0])
print('myDefautModel = ',  myDefautModel)
test_model(myDefautModel)

myModel = ot.AbsoluteExponential([2.0] * spatialDimension, [3.0])
test_model(myModel)

myDefautModel = ot.MaternModel([2.0], [3.0], 1.5)
print('myDefautModel = ',  myDefautModel)
test_model(myDefautModel)

myModel = ot.MaternModel([2.0] * spatialDimension, [3.0], 1.5)
test_model(myModel)


myDefautModel = ot.ExponentiallyDampedCosineModel([2.0], [3.0], 1.5)
print('myDefautModel = ',  myDefautModel)
test_model(myDefautModel)

myModel = ot.ExponentiallyDampedCosineModel([2.0] * spatialDimension, [3.0], 1.5)
test_model(myModel)

myDefautModel = ot.SphericalModel([2.0], [3.0], 4.5)
print('myDefautModel = ',  myDefautModel)
test_model(myDefautModel)

myModel = ot.SphericalModel([2.0] * spatialDimension, [3.0], 4.5)
test_model(myModel)


myDefautModel = ot.DiracCovarianceModel()
print('myDefautModel = ',  myDefautModel)
test_model(myDefautModel)

amplitude = list(map(lambda k: 1.5 + 2.0 * k, range(2)))
myModel = ot.DiracCovarianceModel(spatialDimension, amplitude)
test_model(myModel)


myDefautModel = ot.ProductCovarianceModel()
print('myDefautModel = ',  myDefautModel)
test_model(myDefautModel)

cov1 = ot.AbsoluteExponential([2.0], [3.0])
cov2 = ot.SquaredExponential([2.0], [3.0])
myModel = ot.ProductCovarianceModel([cov1, cov2])
test_model(myModel)

# Collection ==> add covariance models
# Add AbsoluteExponentialModel to the collection
myAbsoluteExponential = ot.AbsoluteExponential([2.0] * spatialDimension, [3.0])
mySquaredExponential = ot.SquaredExponential([2.0] * spatialDimension, [3.0])
# Add exponentialModel to the collection
amplitude = [4.0, 2.0]
scale = [1.0] * spatialDimension
# Define a spatial correlation
spatialCorrelation = ot.CorrelationMatrix(spatialDimension)
spatialCorrelation[1, 0] = 0.3
myExponentialModel = ot.ExponentialModel(scale, amplitude, spatialCorrelation)
# Build TensorizedCovarianceModel with scale = [1,..,1]
myModel = ot.TensorizedCovarianceModel(
    [myAbsoluteExponential, mySquaredExponential, myExponentialModel])
test_model(myModel)
# Define new scale
scale = [2.5, 1.5]
myModel.setScale(scale)
test_model(myModel)
