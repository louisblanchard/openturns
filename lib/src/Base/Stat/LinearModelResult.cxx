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

/* Formula accessor */
String LinearModelResult::getFormula() const
{
  return condensedFormula_;
}

void LinearModelResult::setFormula(const String formula)
{
  condensedFormula_ = formula;
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
