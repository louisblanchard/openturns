//                                               -*- C++ -*-
/**
 *  @brief The result of a linear model estimation
 *
 *  Copyright 2005-2016 Airbus-EDF-IMACS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "openturns/LinearModelResult.hxx"
#include "openturns/PersistentObjectFactory.hxx"
#include "openturns/OSS.hxx"


BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(LinearModelResult);

static const Factory<LinearModelResult> Factory_LinearModelResult;

/* Default constructor */
LinearModelResult::LinearModelResult()
  : MetaModelResult()
{
  // Nothing to do
}

/* Parameter constructor */
LinearModelResult::LinearModelResult(const NumericalSample & inputSample,
    const NumericalSample & outputSample,
    const LinearModel & linearModel,
    const NumericalPoint & residuals,
    const NumericalPoint & relativeErrors)
  : MetaModelResult(NumericalMathFunction(inputSample, outputSample), NumericalMathFunction(), residuals, relativeErrors)
  , inputSample_(inputSample)
  , outputSample_(outputSample)
  , linearModel_(linearModel)
{
  const UnsignedInteger size = inputSample.getSize();
  if (size != outputSample.getSize())
    throw InvalidArgumentException(HERE) << "In LinearModelResult::LinearModelResult, input & output sample have different size. input sample size = " << size << ", output sample size = " << outputSample.getSize();
}

/* Virtual constructor */
LinearModelResult * LinearModelResult::clone() const
{
  return new LinearModelResult(*this);
}


/* String converter */
String LinearModelResult::__repr__() const
{
  return OSS(true) << "class=" << getClassName();
}


/* Linear model accessor */
LinearModel LinearModelResult::getLinearModel() const
{
  return linearModel_;
}


/* Input sample accessor */
NumericalSample LinearModelResult::getInputSample() const
{
  return inputSample_;
}


/* Output sample accessor */
NumericalSample LinearModelResult::getOutputSample() const
{
  return outputSample_;
}


/* Number of degrees of freedom */
UnsignedInteger LinearModelResult::getDegreesOfFreedom() const
{
  throw NotYetImplementedException(HERE);
}

/* Adjusted R-squared test */
TestResult LinearModelResult::getAdjustedRSquared() const
{
  throw NotYetImplementedException(HERE);
}

/* R-squared test */
TestResult LinearModelResult::getRSquared() const
{
  throw NotYetImplementedException(HERE);
}

/* Fisher test */
TestResult LinearModelResult::getFisherTest() const
{
  throw NotYetImplementedException(HERE);
}

/* Kolmogorov-Smirnov normality test */
TestResult LinearModelResult::getKolmogorovSmirnovTest() const
{
  throw NotYetImplementedException(HERE);
}

/* Anderson-Darling normality test */
TestResult LinearModelResult::getAndersonDarlingTest() const
{
  throw NotYetImplementedException(HERE);
}

/* Chi-Squared normality test */
TestResult LinearModelResult::getChiSquaredTest() const
{
  throw NotYetImplementedException(HERE);
}

/* Method that returns the ANOVA table (ANalyse Of VAriance) */
void LinearModelResult::printANOVAtable() const
{
  throw NotYetImplementedException(HERE);
}


/* [1] Draw a plot of residuals versus fitted values */
Graph LinearModelResult::drawResidualsVsFitted() const
{
  throw NotYetImplementedException(HERE);
}

/* [2] a Scale-Location plot of sqrt(| residuals |) versus fitted values */
Graph LinearModelResult::drawScaleLocation() const
{
  throw NotYetImplementedException(HERE);
}

/* [3] a Normal quantiles-quantiles plot of standardized residuals */
Graph LinearModelResult::drawQQplot() const
{
  throw NotYetImplementedException(HERE);
}

/* [4] a plot of Cook's distances versus row labels */
Graph LinearModelResult::drawCookDistance() const
{
  throw NotYetImplementedException(HERE);
}

/* [5] a plot of residuals versus leverages that adds bands corresponding to Cook's distances of 0.5 and 1. */
Graph LinearModelResult::drawResidualsVsLeverages() const
{
  throw NotYetImplementedException(HERE);
}

/* [6] a plot of Cook's distances versus leverage/(1-leverage) */
Graph LinearModelResult::drawCookVsLeverages() const
{
  throw NotYetImplementedException(HERE);
}

/* Method save() stores the object through the StorageManager */
void LinearModelResult::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "inputSample_", inputSample_ );
  adv.saveAttribute( "outputSample_", outputSample_ );
  adv.saveAttribute( "linearModel_", linearModel_ );
}


/* Method load() reloads the object from the StorageManager */
void LinearModelResult::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "inputSample_", inputSample_ );
  adv.loadAttribute( "outputSample_", outputSample_ );
  adv.loadAttribute( "linearModel_", linearModel_ );
}


END_NAMESPACE_OPENTURNS
