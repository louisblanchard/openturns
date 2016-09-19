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
#include <fstream>
#include <cstdlib>

#include "openturns/LinearModelAlgorithm.hxx"
#include "openturns/Path.hxx"
#include "openturns/Exception.hxx"
#include "openturns/OTconfig.hxx"
#include "openturns/Log.hxx"
#include "openturns/Os.hxx"


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
Description LinearModelStepwiseFactory::getInteractions(const Description & monomials, const UnsignedInteger degree) const
{
  throw NotYetImplementedException(HERE);
}

/* Polynomial accessor */
Description LinearModelStepwiseFactory::getPolynomial(const Description & monomial, const UnsignedInteger degree) const
{
  throw NotYetImplementedException(HERE);
}

String LinearModelStepwiseFactory::getPolynomial(const String & monomial, const UnsignedInteger degree) const
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
LinearModelResult LinearModelStepwiseFactory::buildForward(Indices Smin, 
                                                           Indices Smax, 
                                                           const NumericalScalar k)
{
  return build(Smin,Smax,Smin,k)
}

/* Build a linear model using stepwise regression with "backward" search method */
LinearModelResult LinearModelStepwiseFactory::buildBackward(Indices Smax, 
                                                            Indices Smin, 
                                                            const NumericalScalar k) 
{
  return build(Smax,Smin,Smax,k)
}

/* Build a linear model using stepwise regression with "both" search method */
LinearModelResult LinearModelStepwiseFactory::buildBoth(Indices Smin, 
                                                        Indices Smax, 
                                                        Indices Sini, 
                                                        const NumericalScalar k)
{
  return  build(Smin,Smax,Sini,k)
}

/* Build a linear model using stepwise regression */
LinearModelResult LinearModelStepwiseFactory::build(Indices Smin, 
                                                    Indices Smax, 
                                                    Indices Sini,
                                                    const NumericalScalar k)
{
  /* k : the multiple of the degrees of freedom used for the penality 
        - k=2      Akaike   information criterion (AIC)
        - k=log(n) Bayesian information criterion (BIC)  */
  throw NotYetImplementedException(HERE);
}

/* functions to find argmax of the optimal criteria  */ 
NumericalScalar LinearModelStepwiseFactory::evaluateWith(const Indices j)
{
  throw NotYetImplementedException(HERE);
}

NumericalScalar LinearModelStepwiseFactory::evaluateWithout(const Indices j)  
{
  throw NotYetImplementedException(HERE);
}


END_NAMESPACE_OPENTURNS

