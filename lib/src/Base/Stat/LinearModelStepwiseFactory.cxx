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
#include "openturns/SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS


CLASSNAMEINIT(LinearModelStepwiseFactory);

/* Default constructor */
LinearModelStepwiseFactory::LinearModelStepwiseFactory()
  : PersistentObject()
  , variables_(0)
  , direction_(LinearModelStepwiseFactory::BOTH)
  , penalty_(-1.0)
  , maximumIterationNumber_(1000)
{
  // Nothing to do
}

/* Parameters constructor */
LinearModelStepwiseFactory::LinearModelStepwiseFactory(const Description & variables,
                                                       const SignedInteger direction,
                                                       const NumericalScalar penalty,
                                                       const UnsignedInteger maximumIterationNumber)
  : PersistentObject()
  , variables_(variables)
  , direction_(static_cast<LinearModelStepwiseFactory::Direction>(direction))
  , penalty_(penalty)
  , maximumIterationNumber_(maximumIterationNumber)
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
  OSS oss(true);
  oss << "class=" << getClassName()
      << " variables=" << variables_
      << " direction=" << direction_
      << " penalty=" << penalty_
      << " maximumIterationNumber=" << maximumIterationNumber_
      << " condensedFormula=" << condensedFormula_
      << " formulas=" << formulas_;
  return oss;
}

/* String converter */
String LinearModelStepwiseFactory::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class=" << getClassName()
      << " variables=" << variables_
      << " direction=" << direction_
      << " penalty=" << penalty_
      << " maximumIterationNumber=" << maximumIterationNumber_
      << " condensedFormula=" << condensedFormula_
      << " formulas=" << formulas_;
  return oss;
}

/* Get Formula */
String LinearModelStepwiseFactory::getFormula() const
{
  return condensedFormula_;
}

/* Get penalty of the stepwise regression method */
NumericalScalar LinearModelStepwiseFactory::getPenalty() const
{
  return penalty_;
}

/* Get direction of the stepwise regression method */
LinearModelStepwiseFactory::Direction LinearModelStepwiseFactory::getDirection() const
{
  return direction_;
}

/* Set direction of the stepwise regression method */
void LinearModelStepwiseFactory::setDirection(const SignedInteger direction)
{
  switch(direction)
  {
    case -1:
      direction_ = BACKWARD;
      break;
    case 0:
      direction_ = BOTH;
      break;
    case 1:
      direction_ = FORWARD;
      break;
    default:
      throw InvalidArgumentException(HERE) << "Invalid direction argument: " << direction;
  }
}

/* Set penalty of the stepwise regression method  */
void LinearModelStepwiseFactory::setPenalty(const NumericalScalar penalty)
{
  penalty_ = penalty;
}

/* Set maximum number of iterations of the stepwise regression method  */
void LinearModelStepwiseFactory::setMaximumIterationNumber(const NumericalScalar maximumIterationNumber)
{
  maximumIterationNumber_ = maximumIterationNumber;
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

/* TBB functor to speed-up forward insertion index computation */
struct UpdateForwardFunctor
{
  const Indices & indexSet_;
  const Matrix & X_;
  const Matrix & Xmax_;
  const Matrix & epsilon_; // Y - X A Xt Y
  const Matrix & M_;       // X A
  NumericalScalar criterion_;
  UnsignedInteger bestIndex_;

  UpdateForwardFunctor(const Indices & indexSet, const Matrix & X, const Matrix & Xmax, const Matrix & epsilon, const Matrix & M)
    : indexSet_(indexSet), X_(X), Xmax_(Xmax), epsilon_(epsilon), M_(M)
    , criterion_(SpecFunc::MaxNumericalScalar), bestIndex_(0) {}

  UpdateForwardFunctor(const UpdateForwardFunctor & other, TBB::Split)
    : indexSet_(other.indexSet_), X_(other.X_), Xmax_(other.Xmax_), epsilon_(other.epsilon_), M_(other.M_)
    , criterion_(SpecFunc::MaxNumericalScalar), bestIndex_(0) {}

  void operator() (const TBB::BlockedRange<UnsignedInteger> & r)
  {
    throw NotYetImplementedException(HERE);
  } // operator

  void join(const UpdateForwardFunctor & other)
  {
    if (other.criterion_ > criterion_)
    {
      criterion_ = other.criterion_;
      bestIndex_ = other.bestIndex_;
    }
  }
}; /* end struct UpdateForwardFunctor */

/* TBB functor to speed-up backward insertion index computation */
struct UpdateBackwardFunctor
{
  const Indices & indexSet_;
  const Indices & indexXmaxToX_; // position of columns in X_
  const Matrix & X_;
  const Matrix & Y_;
  const Matrix & A_;
  const Matrix B_; // Not a reference because each thread needs its own copy because it modifies B
  NumericalScalar criterion_;
  UnsignedInteger bestIndex_;

  UpdateBackwardFunctor(const Indices & indexSet, const Indices & indexXmaxToX, const Matrix & X, const Matrix & Y, const Matrix & A, const Matrix & B)
    : indexSet_(indexSet), indexXmaxToX_(indexXmaxToX), X_(X), Y_(Y), A_(A), B_(B)
    , criterion_(SpecFunc::MaxNumericalScalar), bestIndex_(0) {}

  UpdateBackwardFunctor(const UpdateBackwardFunctor & other, TBB::Split)
    : indexSet_(other.indexSet_), indexXmaxToX_(other.indexXmaxToX_), X_(other.X_), Y_(other.Y_), A_(other.A_), B_(other.B_)
    , criterion_(other.criterion_), bestIndex_(other.bestIndex_) {}

  void operator() (const TBB::BlockedRange<UnsignedInteger> & r)
  {
    throw NotYetImplementedException(HERE);
  } // operator

  void join(const UpdateBackwardFunctor & other)
  {
    if (other.criterion_ > criterion_)
    {
      criterion_ = other.criterion_;
      bestIndex_ = other.bestIndex_;
    }
  }
}; /* end struct UpdateBackwardFunctor */

/* Build a linear model using stepwise regression */
LinearModelResult LinearModelStepwiseFactory::build(const NumericalSample & inputSample,
                                                    const NumericalSample & outputSample,
                                                    const Indices & minimalIndices,
                                                    const Indices & startIndices)
{
  throw NotYetImplementedException(HERE);
}

/* Compute the likelihood function */
NumericalScalar LinearModelStepwiseFactory::computeLogLikelihood()
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

