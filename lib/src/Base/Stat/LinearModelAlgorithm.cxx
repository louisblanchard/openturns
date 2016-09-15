//                                               -*- C++ -*-
/**
 *  @brief LinearModelAlgorithm implements the linear model
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
#include "openturns/LinearModelAlgorithm.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(LinearModelAlgorithm);

/* Default constructor */
LinearModelAlgorithm::LinearModelAlgorithm()
  : PersistentObject()
  , inputSample_(0, 0)
  , outputSample_(0, 0)
  , result_()
  , hasRun_(false)
{
  // Nothing to do
}

/* Parameters constructor */
LinearModelAlgorithm::LinearModelAlgorithm(const NumericalSample & inputSample,
    const NumericalSample & outputSample)
  : PersistentObject()
  , inputSample_(0, 0)
  , outputSample_(0, 0)
  , result_()
  , hasRun_(false)
{
  // Check the sample sizes
  if (inputSample.getSize() != outputSample.getSize())
    throw InvalidArgumentException(HERE) << "In LinearModelAlgorithm::LinearModelAlgorithm, input sample size (" << inputSample.getSize() << ") does not match output sample size (" << outputSample.getSize() << ").";
  // Set samples
  inputSample_ = inputSample;
  outputSample_ = outputSample;
}


/* Virtual constructor */
LinearModelAlgorithm * LinearModelAlgorithm::clone() const
{
  return new LinearModelAlgorithm(*this);
}


/* Perform regression */
void LinearModelAlgorithm::run()
{
  // Do not run again if already computed
  if (hasRun_) return;
  throw NotYetImplementedException(HERE);
  hasRun_ = true;
}


/* String converter */
String LinearModelAlgorithm::__repr__() const
{
  OSS oss;
  oss << "class=" << getClassName()
      << ", inputSample=" << inputSample_
      << ", outputSample=" << outputSample_
      << ", result=" << result_;
  return oss;
}


NumericalSample LinearModelAlgorithm::getInputSample() const
{
  return inputSample_;
}


NumericalSample LinearModelAlgorithm::getOutputSample() const
{
  return outputSample_;
}


LinearModelResult LinearModelAlgorithm::getResult()
{
  run();
  return result_;
}


/* Method save() stores the object through the StorageManager */
void LinearModelAlgorithm::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "inputSample_", inputSample_ );
  adv.saveAttribute( "outputSample_", outputSample_ );
  adv.saveAttribute( "result_", result_ );
}

/* Method load() reloads the object from the StorageManager */
void LinearModelAlgorithm::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "inputSample_", inputSample_ );
  adv.loadAttribute( "outputSample_", outputSample_ );
  adv.loadAttribute( "result_", result_ );
}

END_NAMESPACE_OPENTURNS
