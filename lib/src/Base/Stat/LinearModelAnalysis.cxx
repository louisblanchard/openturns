//                                               -*- C++ -*-
/**
 *  @brief The linear model analysis
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
#include "openturns/LinearModelAnalysis.hxx"
#include "openturns/PersistentObjectFactory.hxx"
#include "openturns/OSS.hxx"


BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(LinearModelAnalysis);

static const Factory<LinearModelAnalysis> Factory_LinearModelAnalysis;

/* Default constructor */
LinearModelAnalysis::LinearModelAnalysis()
  : PersistentObject()
{
  // Nothing to do
}

/* Parameter constructor */
LinearModelAnalysis::LinearModelAnalysis(const LinearModelResult & linearModelResult)
  : PersistentObject()
  , linearModelResult_(linearModelResult)
{
  // Nothing to do
}

/* Virtual constructor */
LinearModelAnalysis * LinearModelAnalysis::clone() const
{
  return new LinearModelAnalysis(*this);
}


/* String converter */
String LinearModelAnalysis::__repr__() const
{
  return OSS(true) << "class=" << getClassName();
}


/* Method that returns the ANOVA table (ANalyse Of VAriance) */
String LinearModelAnalysis::__str__() const
{
  throw NotYetImplementedException(HERE);
}


/* Linear model accessor */
LinearModelResult LinearModelAnalysis::getLinearModelResult() const
{
  return linearModelResult_;
}

String LinearModelAnalysis::getFormula() const
{
  return linearModelResult_.getFormula();
}

Description LinearModelAnalysis::getCoefficientsNames() const
{
  return linearModelResult_.getCoefficientsNames();
}

NumericalSample LinearModelAnalysis::getResiduals() const
{
  return linearModelResult_.getSampleResiduals();
}

NumericalSample LinearModelAnalysis::getStandardizedResiduals() const
{
  return linearModelResult_.getStandardizedResiduals();
}

NumericalSample LinearModelAnalysis::getCoefficientsEstimates() const
{
  LinearModel model = linearModelResult_.getLinearModel();
  NumericalPoint regression(model.getRegression());
  NumericalSample result(regression.getDimension(), 1);
  for (UnsignedInteger i = 0; i < regression.getDimension(); ++i)
  {
    result(i, 0) = regression[i];
  }
}

NumericalSample LinearModelAnalysis::getCoefficientsStandardErrors() const
{
  throw NotYetImplementedException(HERE);
}

NumericalSample LinearModelAnalysis::getCoefficientsTScores() const
{
  throw NotYetImplementedException(HERE);
}

NumericalSample LinearModelAnalysis::getCoefficientsPValues() const
{
  throw NotYetImplementedException(HERE);
}

/* Leverages */
NumericalPoint LinearModelAnalysis::getLeverages() const
{
  throw NotYetImplementedException(HERE);
}

/* Cook's distances */
NumericalPoint LinearModelAnalysis::getCookDistances() const
{
  throw NotYetImplementedException(HERE);
}

/* Number of degrees of freedom */
UnsignedInteger LinearModelAnalysis::getDegreesOfFreedom() const
{
  throw NotYetImplementedException(HERE);
}

/* R-squared test */
NumericalScalar LinearModelAnalysis::getRSquared() const
{
  throw NotYetImplementedException(HERE);
}

/* Adjusted R-squared test */
NumericalScalar LinearModelAnalysis::getAdjustedRSquared() const
{
  throw NotYetImplementedException(HERE);
}

/* Fisher test */
NumericalScalar LinearModelAnalysis::getFisherScore() const
{
  throw NotYetImplementedException(HERE);
}

NumericalScalar LinearModelAnalysis::getFisherPValue() const
{
  throw NotYetImplementedException(HERE);
}

/* Kolmogorov-Smirnov normality test */
TestResult LinearModelAnalysis::getNormalityTestResultKolmogorovSmirnov() const
{
  throw NotYetImplementedException(HERE);
}

/* Anderson-Darling normality test */
TestResult LinearModelAnalysis::getNormalityTestResultAndersonDarling() const
{
  throw NotYetImplementedException(HERE);
}

/* Chi-Squared normality test */
TestResult LinearModelAnalysis::getNormalityTestResultChiSquared() const
{
  throw NotYetImplementedException(HERE);
}

/* [1] Draw a plot of residuals versus fitted values */
Graph LinearModelAnalysis::drawResidualsVsFitted() const
{
  throw NotYetImplementedException(HERE);
}

/* [2] a Scale-Location plot of sqrt(| residuals |) versus fitted values */
Graph LinearModelAnalysis::drawScaleLocation() const
{
  throw NotYetImplementedException(HERE);
}

/* [3] a Normal quantiles-quantiles plot of standardized residuals */
Graph LinearModelAnalysis::drawQQplot() const
{
  throw NotYetImplementedException(HERE);
}

/* [4] a plot of Cook's distances versus row labels */
Graph LinearModelAnalysis::drawCookDistance() const
{
  throw NotYetImplementedException(HERE);
}

/* [5] a plot of residuals versus leverages that adds bands corresponding to Cook's distances of 0.5 and 1. */
Graph LinearModelAnalysis::drawResidualsVsLeverages() const
{
  throw NotYetImplementedException(HERE);
}

/* [6] a plot of Cook's distances versus leverage/(1-leverage) */
Graph LinearModelAnalysis::drawCookVsLeverages() const
{
  throw NotYetImplementedException(HERE);
}

/* Method save() stores the object through the StorageManager */
void LinearModelAnalysis::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "linearModelResult_", linearModelResult_ );
}


/* Method load() reloads the object from the StorageManager */
void LinearModelAnalysis::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "linearModelResult_", linearModelResult_ );
}


END_NAMESPACE_OPENTURNS
