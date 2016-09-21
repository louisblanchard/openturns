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
  , inputSample_(0, 0)
{
  // Nothing to do
}

/* Parameters constructor */
LinearModelStepwiseFactory::LinearModelStepwiseFactory(const NumericalSample & inputSample)
  : PersistentObject()
  , inputSample_(0, 0)
{
  // Set samples
  inputSample_ = inputSample;
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

/* Sample accessor */
NumericalSample LinearModelStepwiseFactory::getInputSample() const
{
  return inputSample_;
}

/* formula accessor */
String LinearModelStepwiseFactory::getFormula() const
{
  return formula_;
}

/* Interactions accessor */
Description LinearModelStepwiseFactory::getInteractions(const Description & variables, const UnsignedInteger degree) const
{
  throw NotYetImplementedException(HERE);
}

/* Polynomial accessor */
Description LinearModelStepwiseFactory::getPolynomial(const Description & variables, const UnsignedInteger degree) const
{
  throw NotYetImplementedException(HERE);
}

String LinearModelStepwiseFactory::getPolynomial(const String & variable, const UnsignedInteger degree) const
{
  throw NotYetImplementedException(HERE);
}


/* columns Indices accessor */
Indices LinearModelStepwiseFactory::getIndices(const Description & indice) const
{
  throw NotYetImplementedException(HERE);
}

/* Add methods */
void LinearModelStepwiseFactory::add(const Description & name)
{
  throw NotYetImplementedException(HERE);
}

void LinearModelStepwiseFactory::add(const String & name)
{
  throw NotYetImplementedException(HERE);
}

/* Build a linear model using stepwise regression with "forward" search method */
LinearModelResult LinearModelStepwiseFactory::buildForward(const Indices & minimalIndices,
                                                           const NumericalScalar k,
                                                           const UnsignedInteger itermax)
{
  return build(minimalIndices, minimalIndices, true, false, k, itermax);
}

/* Build a linear model using stepwise regression with "backward" search method */
LinearModelResult LinearModelStepwiseFactory::buildBackward(const Indices & minimalIndices,
                                                            const NumericalScalar k,
                                                            const UnsignedInteger itermax)
{
  Indices startIndices(monomials_.getSize());
  startIndices.fill();
  return build(minimalIndices, startIndices, false, true, k, itermax);
}

/* Build a linear model using stepwise regression with "both" search method */
LinearModelResult LinearModelStepwiseFactory::buildBoth(const Indices & minimalIndices,
                                                        const Indices & startIndices,
                                                        const NumericalScalar k,
                                                        const UnsignedInteger itermax)
{
  return build(minimalIndices, startIndices, true, true, k, itermax);
}

/* Build a linear model using stepwise regression */
LinearModelResult LinearModelStepwiseFactory::build(const Indices & minimalIndices,
                                                    const Indices & startIndices,
                                                    const Bool forward,
                                                    const Bool backward,
                                                    const NumericalScalar k,
                                                    const UnsignedInteger itermax)
{
  /* k : the multiple of the degrees of freedom used for the penality
        - k=2      Akaike   information criterion (AIC)
        - k=log(n) Bayesian information criterion (BIC)  */
  throw NotYetImplementedException(HERE);
}

/* functions to find argmax of the optimal criteria  */
NumericalScalar LinearModelStepwiseFactory::evaluateWith(const Indices & j)
{
  throw NotYetImplementedException(HERE);
}

NumericalScalar LinearModelStepwiseFactory::evaluateWithout(const Indices & j)
{
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

