//                                               -*- C++ -*-
/**
 *  @brief Factory for linear model
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
#include "openturns/LinearModelStepwiseFactory.hxx"
#include "openturns/Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS


CLASSNAMEINIT(LinearModelStepwiseFactory);

/* Default constructor */
LinearModelStepwiseFactory::LinearModelStepwiseFactory()
  : PersistentObject()
  , variables_(0)
  , inputSample_(0, 0)
  , outputSample_(0, 0)
{
  // Nothing to do
}

/* Parameters constructor */
LinearModelStepwiseFactory::LinearModelStepwiseFactory(const Description & variables)
  : PersistentObject()
  , variables_(variables)
  , inputSample_(0, 0)
  , outputSample_(0, 0)
{
  // Nothing to do
}


/* Virtual constructor */
LinearModelStepwiseFactory * LinearModelStepwiseFactory::clone() const
{
  return new LinearModelStepwiseFactory(*this);
}


/* String converter */
String LinearModelStepwiseFactory::__repr__() const
{
  return OSS(true) << "class=" << getClassName();
}

/* String converter */
String LinearModelStepwiseFactory::__str__(const String & offset) const
{
  return OSS(false) << "class=" << getClassName();
}

/* Formula accessor */
String LinearModelStepwiseFactory::getFormula() const
{
  return condensedFormula_;
}

/* Get formulas of interactions between variables */
Description LinearModelStepwiseFactory::getInteractions(const UnsignedInteger degree, const Description & variables) const
{
  throw NotYetImplementedException(HERE);
}

/* Get formulas of monomials */
Description LinearModelStepwiseFactory::getPolynomial(const UnsignedInteger degree, const Description & variables) const
{
  throw NotYetImplementedException(HERE);
}

String LinearModelStepwiseFactory::getPolynomial(const UnsignedInteger degree, const String & variable) const
{
  throw NotYetImplementedException(HERE);
}


/* Get column indices of given formulas */
Indices LinearModelStepwiseFactory::getIndices(const Description & formulas) const
{
  throw NotYetImplementedException(HERE);
}

/* Add formulas */
void LinearModelStepwiseFactory::add(const Description & formulas)
{
  throw NotYetImplementedException(HERE);
}

void LinearModelStepwiseFactory::add(const String & formula)
{
  throw NotYetImplementedException(HERE);
}

/* Build a linear model using stepwise regression */
LinearModelResult LinearModelStepwiseFactory::build(const NumericalSample & inputSample,
    const NumericalSample & outputSample,
    const UnsignedInteger direction,
    const Indices & minimalIndices,
    const Indices & startIndices,
    const NumericalScalar k,
    const UnsignedInteger maximumIterationNumber)
{
  /* k : the multiple of the degrees of freedom used for the penality
        - k=2      Akaike   information criterion (AIC)
        - k=log(n) Bayesian information criterion (BIC)  */
  throw NotYetImplementedException(HERE);
}

/* Compute the likelihood function */
NumericalScalar LinearModelStepwiseFactory::computeLogLikelihood()
{
  throw NotYetImplementedException(HERE);
}

/* Compute the likelihood function for stepwise regression with "forward" search method */
NumericalScalar LinearModelStepwiseFactory::computeLogLikelihoodForward(const UnsignedInteger j)
{
  throw NotYetImplementedException(HERE);
}

/* Compute the likelihood function for stepwise regression with "backward" search method */
NumericalScalar LinearModelStepwiseFactory::computeLogLikelihoodBackward(const UnsignedInteger j)
{
  throw NotYetImplementedException(HERE);
}

/* Method save() stores the object through the StorageManager */
void LinearModelStepwiseFactory::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  throw NotYetImplementedException(HERE);
}

/* Method load() reloads the object from the StorageManager */
void LinearModelStepwiseFactory::load(Advocate & adv)
{
  PersistentObject::load(adv);
  throw NotYetImplementedException(HERE);
}



END_NAMESPACE_OPENTURNS

