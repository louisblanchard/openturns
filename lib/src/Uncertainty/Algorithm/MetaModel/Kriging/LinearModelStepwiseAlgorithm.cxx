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
#include "openturns/LinearModelStepwiseAlgorithm.hxx"
#include "openturns/NumericalMathFunction.hxx"
#include "openturns/Exception.hxx"
#include "openturns/Combinations.hxx"
#include "openturns/SpecFunc.hxx"
#include "openturns/Log.hxx"

#include <cmath>

BEGIN_NAMESPACE_OPENTURNS


CLASSNAMEINIT(LinearModelStepwiseAlgorithm);

/* Default constructor */
LinearModelStepwiseAlgorithm::LinearModelStepwiseAlgorithm()
  : PersistentObject()
  , direction_(BOTH)
  , penalty_(-1.0)
  , maximumIterationNumber_(1000)
  , condensedFormula_("1")
  , formulas_(1, "1")
  , hasRun_(false)
{
  // Nothing to do
}

/* Parameters constructor */
LinearModelStepwiseAlgorithm::LinearModelStepwiseAlgorithm(const NumericalSample & inputSample,
                                                           const NumericalSample & outputSample,
                                                           const SignedInteger direction,
                                                           const NumericalScalar penalty,
                                                           const UnsignedInteger maximumIterationNumber)
  : PersistentObject()
  , inputSample_(inputSample)
  , outputSample_(outputSample)
  , direction_(static_cast<LinearModelStepwiseAlgorithm::Direction>(direction))
  , penalty_(penalty)
  , maximumIterationNumber_(maximumIterationNumber)
  , condensedFormula_("1")
  , formulas_(1, "1")
  , hasRun_(false)
{
  // Nothing to do
}


/* Virtual constructor */
LinearModelStepwiseAlgorithm * LinearModelStepwiseAlgorithm::clone() const
{
  return new LinearModelStepwiseAlgorithm(*this);
}


/* String converter */
String LinearModelStepwiseAlgorithm::__repr__() const
{
  OSS oss(true);
  oss << "class=" << getClassName()
      << " direction=" << direction_
      << " penalty=" << penalty_
      << " maximumIterationNumber=" << maximumIterationNumber_
      << " condensedFormula=" << condensedFormula_
      << " formulas=" << formulas_;
  return oss;
}

/* String converter */
String LinearModelStepwiseAlgorithm::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class=" << getClassName()
      << " direction=" << direction_
      << " penalty=" << penalty_
      << " maximumIterationNumber=" << maximumIterationNumber_
      << " condensedFormula=" << condensedFormula_
      << " formulas=" << formulas_;
  return oss;
}

/* Sample accessors */
NumericalSample LinearModelStepwiseAlgorithm::getInputSample() const
{
  return inputSample_;
}

NumericalSample LinearModelStepwiseAlgorithm::getOutputSample() const
{
  return outputSample_;
}

/* Direction accessors */
LinearModelStepwiseAlgorithm::Direction LinearModelStepwiseAlgorithm::getDirection() const
{
  return direction_;
}

/* Set direction of the stepwise regression method */
void LinearModelStepwiseAlgorithm::setDirection(const SignedInteger direction)
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
  hasRun_ = false;
}


/* Penalty accessors */
NumericalScalar LinearModelStepwiseAlgorithm::getPenalty() const
{
  return penalty_;
}

void LinearModelStepwiseAlgorithm::setPenalty(const NumericalScalar penalty)
{
  penalty_ = penalty;
  hasRun_ = false;
}

/* Maximum number of iterations accessors */
UnsignedInteger LinearModelStepwiseAlgorithm::getMaximumIterationNumber() const
{
  return maximumIterationNumber_;
}

void LinearModelStepwiseAlgorithm::setMaximumIterationNumber(const UnsignedInteger maximumIterationNumber)
{
  maximumIterationNumber_ = maximumIterationNumber;
  hasRun_ = false;
}

/* Formula accessor */
String LinearModelStepwiseAlgorithm::getFormula() const
{
  return condensedFormula_;
}

/* Add formulas */
void LinearModelStepwiseAlgorithm::add(const Description & formulas)
{
  for (Description::const_iterator it = formulas.begin(); it != formulas.end(); ++it)
    add(*it);
}

void LinearModelStepwiseAlgorithm::add(const String & formula)
{
  for (Description::const_iterator it = formulas_.begin(); it != formulas_.end(); ++it)
  {
    if (*it == formula) return;
  }
  formulas_.add(formula);
  condensedFormula_ += " + " + formula;
}

void LinearModelStepwiseAlgorithm::add(const NumericalSample & userColumns)
{
  userColumns_.stack(userColumns);
  const Description userFormulas(userColumns.getDescription());
  for (Description::const_iterator it = userFormulas.begin(); it != userFormulas.end(); ++it)
    condensedFormula_ += " + " + *it;
}

/* Remove formulas */
void LinearModelStepwiseAlgorithm::remove(const Description & formulas)
{
  std::set<String> deletedFormulas;
  for (Description::const_iterator it = formulas.begin(); it != formulas.end(); ++it)
    deletedFormulas.insert(*it);
  const UnsignedInteger size = formulas_.getSize();
  Description newFormulas(size - formulas.getSize());
  for (Description::const_iterator it = formulas_.begin(); it != formulas_.end(); ++it)
  {
    if (deletedFormulas.find(*it) != deletedFormulas.end())
      newFormulas.add(*it);
  }
  formulas_ = newFormulas;
  for (Description::const_iterator it = formulas.begin(); it != formulas.end(); ++it)
    condensedFormula_ += " - " + *it;
}

void LinearModelStepwiseAlgorithm::remove(const Indices & columns)
{
  for (Indices::const_iterator it = columns.begin(); it != columns.end(); ++it)
    condensedFormula_ += " - " + formulas_[*it];

  const UnsignedInteger size = formulas_.getSize();
  Indices newFormulaIndices(size);
  newFormulaIndices.fill();
  for (Indices::const_iterator it = columns.begin(); it != columns.end(); ++it)
    newFormulaIndices[*it] = size;
  Description newFormulas(size - columns.getSize());
  UnsignedInteger index = 0;
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    if (newFormulaIndices[i] != size)
    {
      newFormulas[index] = formulas_[i];
      ++index;
    }
  }
  formulas_ = newFormulas;
}

/* Get column indices of given formulas */
Indices LinearModelStepwiseAlgorithm::getIndices(const Description & formulas) const
{
  Indices result;
  const UnsignedInteger size = formulas_.getSize();
  for (Description::const_iterator it = formulas.begin(); it != formulas.end(); ++it)
  {
    const String formula = *it;
    for (UnsignedInteger i = 0; i < size; ++i)
    {
      if (formula == formulas_[i])
      {
        result.add(i);
        break;
      }
    }
  }
  return result;
}

/* Interactions between variables */
Description LinearModelStepwiseAlgorithm::getInteractions(const UnsignedInteger degree, const Description & variables) const
{
  Description result(1, "1");
  if (degree == 0) return result;

  const Description input(variables.isEmpty() ? inputSample_.getDescription() : variables);
  result.add(input);
  if (degree == 1) return result;

  for (UnsignedInteger n = 2; n <= degree; ++n)
    result.add(getPower(n, input));

  for (UnsignedInteger n = 2; n <= degree; ++n)
  {
    Combinations c(n, input.getSize());
    Combinations::IndicesCollection coll(c.generate());
    for (Combinations::IndicesCollection::const_iterator it = coll.begin(); it != coll.end(); ++it)
    {
      const Indices indices(*it);
      String accumulated(input[indices[0]]);
      for (UnsignedInteger i = 1; i < indices.getSize(); ++i)
        accumulated += "*" + input[indices[i]];
      result.add(accumulated);
    }
  }
  return result;
}
void LinearModelStepwiseAlgorithm::addInteractions(const UnsignedInteger degree, const Description & variables)
{
  add(getInteractions(degree, variables));
}
void LinearModelStepwiseAlgorithm::removeInteractions(const UnsignedInteger degree, const Description & variables)
{
  remove(getInteractions(degree, variables));
}

/* Power of variables */
Description LinearModelStepwiseAlgorithm::getPower(const UnsignedInteger degree, const Description & variables) const
{
  if (degree == 0) return Description(1, "1");
  const Description input(variables.isEmpty() ? inputSample_.getDescription() : variables);
  if (degree == 1) return input;

  Description result;
  for (Description::const_iterator it = input.begin(); it != input.end(); ++it)
  {
    result.add(OSS() << *it << "^" << degree);
  }
  return result;
}
void LinearModelStepwiseAlgorithm::addPower(const UnsignedInteger degree, const Description & variables)
{
  add(getPower(degree, variables));
}
void LinearModelStepwiseAlgorithm::removePower(const UnsignedInteger degree, const Description & variables)
{
  remove(getPower(degree, variables));
}

/* Set indices of minimal model */
void LinearModelStepwiseAlgorithm::setMinimalIndices(const Indices & minimalIndices)
{
  minimalIndices_ = minimalIndices;
  hasRun_ = false;
}

/* Set indices of start model */
void LinearModelStepwiseAlgorithm::setStartIndices(const Indices & startIndices)
{
  startIndices_ = startIndices;
  hasRun_ = false;
}


/*
  logLikelihood(\hat\beta, \hat\sigma | Y) = -(n/2) ( log(2\pi) + log(\hat\sigma^2) + 1)
  where
     \hat\sigma^2 = (1/n) (Y - HY) (Y - HY)^T
                H = X (X^T X)^{-1} X^T

  When (X^T X)^{-1} has been computed, there are update methods to compute
  the inverse when a column is added to or removed from X.
*/

/* TBB functor to speed-up backward insertion index computation

    If X is augmented by one column:
      X_+ = (X x_+)
                         / A + (1/c) D D^T |  -(1/c) D \
      (X_+ X^T_+)^{-1} = |-----------------+-----------|
                         \      -(1/c) D^T |   (1/c)   /
     with   D = A X^T x_+
            c = x_+^T x_+ - x_+^T X D

    By definition,
      H_+ = X_+ (X_+^T X_+)^{-1} X_+^T
    And thus
      H_+ Y = X A X^T Y + (1/(x_+^T x_+ - x_+^T X A X^T x_+))*(
                       X A X^T x_+ x_+^T X A X^T
                     - X A X^T x_+ x_+^T
                     - x_+ x_+^T X A X^T
                     + x_+ x_+^T)
    We compute
      M = X A
      B = X^T Y
      residual = Y - X A X^T Y = Y - M B
    and for each column j in Smax \ S*,
        b_j = X^T x_j
        d_j = M b_j
        v_j = x_j - d_j
        Y - H_j Y = residual - (x_j^T residual /(x_j^T v_j)) v_j
*/
struct UpdateForwardFunctor
{
  const Indices & indexSet_;
  const Matrix & X_;
  const Matrix & Xmax_;
  const Matrix & residual_;
  const Matrix & M_;
  NumericalScalar criterion_;
  UnsignedInteger bestIndex_;

  UpdateForwardFunctor(const Indices & indexSet, const Matrix & X, const Matrix & Xmax, const Matrix & residual, const Matrix & M)
    : indexSet_(indexSet), X_(X), Xmax_(Xmax), residual_(residual), M_(M)
    , criterion_(SpecFunc::MaxNumericalScalar), bestIndex_(Xmax.getNbColumns()) {}

  UpdateForwardFunctor(const UpdateForwardFunctor & other, TBB::Split)
    : indexSet_(other.indexSet_), X_(other.X_), Xmax_(other.Xmax_), residual_(other.residual_), M_(other.M_)
    , criterion_(other.criterion_), bestIndex_(other.bestIndex_) {}

  void operator() (const TBB::BlockedRange<UnsignedInteger> & r)
  {
    const UnsignedInteger size(Xmax_.getNbRows());
    Matrix xi(size, 1);
    NumericalPoint xiNP(size);
    NumericalPoint viNP(size);
    NumericalPoint residualNP(size);
    memcpy(&residualNP[0], &residual_(0, 0), sizeof(NumericalScalar)*size);

    for (UnsignedInteger index = r.begin(); index != r.end(); ++index)
    {
      const UnsignedInteger i = indexSet_[index];
      memcpy(&xi(0, 0), &Xmax_(0, i), sizeof(NumericalScalar)*size);
      const Matrix bi(X_.getImplementation()->genProd(*(xi.getImplementation()), true, false));
      const Matrix di(M_ * bi);
      const Matrix vi(xi - di);
      memcpy(&viNP[0], &vi(0, 0), sizeof(NumericalScalar)*size);
      memcpy(&xiNP[0], &xi(0, 0), sizeof(NumericalScalar)*size);
      const NumericalScalar denominator = dot(xiNP, viNP);
      if (denominator == 0.0) continue;
      const NumericalScalar alpha = dot(xiNP, residualNP) / denominator;
      const NumericalPoint newResidual(residualNP - alpha * viNP);
      const NumericalScalar newCriterion(newResidual.normSquare());
      LOGDEBUG(OSS() << "Squared residual norm when adding column " << i << ": " << newCriterion);
      if (newCriterion < criterion_)
      {
        criterion_ = newCriterion;
        bestIndex_ = i;
      }
    }
  }

  void join(const UpdateForwardFunctor & other)
  {
    if (other.criterion_ < criterion_)
    {
      criterion_ = other.criterion_;
      bestIndex_ = other.bestIndex_;
    }
  }
}; /* end struct UpdateForwardFunctor */

/* TBB functor to speed-up backward insertion index computation

    If column i is removed from X:
      X_{-i} = X where column i is removed
      A_{-i,-i} = A where column and row i are removed
                = (a_{jk})_{j,k=1..p, j<>i, k<>i}
      A_{i,-i} = (a_{ij})_{j=1..p, j<>i}
    It can be shown that
      (X_{-i}^T X_{-i})^{-1} = A_{-i,-i} - (1/a_{ii}) A_{-i,i} A_{i,-i}
    And thus
      H_- = X_{-i} (X_{-i}^T X_{-i})^{-1} X_{-i}^T
      H_- Y = X_{-i} (X_{-i}^T X_{-i})^{-1} X_{-i}^T Y
            = X_{-i} A_{-i,-i} X_{-i}^T - (1/a_{ii})  X_{-i} A_{-i,i} A_{i,-i} X_{-i}^T Y

    In order to reduce data copies, we do not compute X_{-i} and A_{-i,-i},
    but emulate the same operations by putting zeros at the right places.
    We denote [Q]_{i=0} matrix Q where i-th row is replaced by zeros.
      X_{-i} A_{-i,-i} = ([X]_{i=0} A)_{-i}
    Instead of
      H_- Y = X_{-i} A_{-i,-i} X_{-i}^T - (1/a_{ii})  X_{-i} A_{-i,i} A_{i,-i} X_{-i}^T Y
    we write
      H_- Y = [X]_{i=0} A [X^T Y]_{i=0} - (1/a_{ii})  [X]_{i=0} A_{,i} A_{i,} [X^T Y]_{i=0}
    The final trick is to replace [X]_{i=0} by X in top-left multiplications, and throw i-th
    coefficient and replace it by 0.

    We compute
      B = X^T Y
    and for each column j in S* \ Smin,
      b_j = [B]_{j=0}
      e_j = A b_j
      f_j = e_j - ((e_j)_j/a_{jj}) A_{,j}
      Y - H_- Y = Y - X f_j
    Note that j in S* \ Smin refers to columns in Xmax, we need an array to store
    positions of these columns in X.
 */
struct UpdateBackwardFunctor
{
  const Indices & indexSet_;
  const Indices & columnMaxToCurrent_; // position of maxX_ columns in X_
  const Indices & columnCurrentToMax_; // position of X_ columns in maxX_
  const Matrix & X_;
  const Matrix & Y_;
  const Matrix & A_;
  Matrix B_; // Not a reference because each thread needs its own copy, B is modified
  NumericalScalar criterion_;
  UnsignedInteger bestIndex_;

  UpdateBackwardFunctor(const Indices & indexSet, const Indices & columnMaxToCurrent, const Indices & columnCurrentToMax,
                        const Matrix & X, const Matrix & Y, const Matrix & A, const Matrix & B)
    : indexSet_(indexSet), columnMaxToCurrent_(columnMaxToCurrent), columnCurrentToMax_(columnCurrentToMax)
    , X_(X), Y_(Y), A_(A), B_(B)
    , criterion_(SpecFunc::MaxNumericalScalar), bestIndex_(A.getNbColumns()) {}

  UpdateBackwardFunctor(const UpdateBackwardFunctor & other, TBB::Split)
    : indexSet_(other.indexSet_), columnMaxToCurrent_(other.columnMaxToCurrent_), columnCurrentToMax_(other.columnCurrentToMax_)
    , X_(other.X_), Y_(other.Y_), A_(other.A_), B_(other.B_)
    , criterion_(other.criterion_), bestIndex_(other.bestIndex_) {}

  void operator() (const TBB::BlockedRange<UnsignedInteger> & r)
  {
    const UnsignedInteger size(X_.getNbRows());
    const UnsignedInteger p(B_.getNbRows());
    NumericalPoint yNP(size);
    memcpy(&yNP[0], &Y_(0, 0), sizeof(NumericalScalar)*size);
    NumericalPoint bNP(p);
    memcpy(&bNP[0], &B_(0, 0), sizeof(NumericalScalar)*p);
    NumericalPoint aiNP(p);
    NumericalPoint eiNP(p);
    Matrix fiM(p, 1);
    NumericalPoint newResidualNP(size);

    for (UnsignedInteger index = r.begin(); index != r.end(); ++index)
    {
      const UnsignedInteger iMax = indexSet_[index];
      const UnsignedInteger i = columnMaxToCurrent_[iMax];
      const NumericalScalar Bi = B_(i, 0);
      B_(i, 0) = 0.0;
      bNP[i] = 0.0;
      const Matrix ei(A_ * B_);
      memcpy(&aiNP[0], &A_(0, i), sizeof(NumericalScalar)*p);
      memcpy(&eiNP[0], &ei(0, 0), sizeof(NumericalScalar)*p);
      const NumericalPoint fi(eiNP - (ei(i,0)/A_(i, i)) * aiNP);
      memcpy(&fiM(0, 0), &fi[0], sizeof(NumericalScalar)*p);
      const Matrix newResidual(Y_ - X_ * fiM);
      memcpy(&newResidualNP[0], &newResidual(0, 0), sizeof(NumericalScalar)*size);
      const NumericalScalar newCriterion(newResidualNP.normSquare());
      LOGDEBUG(OSS() << "Squared residual norm when removing column " << iMax << ": " << newCriterion);
      if (newCriterion < criterion_)
      {
        criterion_ = newCriterion;
        bestIndex_ = iMax;
      }
      B_(i, 0) = Bi;
      bNP[i] = Bi;
    }
  } // operator

  void join(const UpdateBackwardFunctor & other)
  {
    if (other.criterion_ < criterion_)
    {
      criterion_ = other.criterion_;
      bestIndex_ = other.bestIndex_;
    }
  }
}; /* end struct UpdateBackwardFunctor */

/* Perform regression */
void LinearModelStepwiseAlgorithm::run()
{
  if (hasRun_) return;

  if (outputSample_.getDimension() != 1)
    throw InvalidArgumentException(HERE) << "Error: cannot perform step method based on output sample of dimension different from 1.";
  if (inputSample_.getSize() != outputSample_.getSize())
    throw InvalidArgumentException(HERE) << "Error: the size of the output sample=" << outputSample_.getSize() << " is different from the size of the input sample=" << inputSample_.getSize();

  LOGDEBUG(OSS() << "Running LinearModelStepwiseAlgorithm " << __str__());
  const UnsignedInteger size(inputSample_.getSize());
  Y_ = Matrix(size, 1, outputSample_.getImplementation()->getData());
  NumericalMathFunction f(inputSample_.getDescription(), formulas_);
  NumericalSample fx(f(inputSample_));
  Matrix Xt(fx.getDimension(), size, fx.getImplementation()->getData());
  maxX_ = Xt.transpose();
  Indices columnMaxToCurrent(maxX_.getNbColumns(), maxX_.getNbColumns());

  if (startIndices_.getSize() == 0)
  {
    switch(direction_)
    {
      case BACKWARD:
        currentX_ = maxX_;
        currentIndices_ = Indices(maxX_.getNbColumns());
        currentIndices_.fill();
        columnMaxToCurrent = currentIndices_;
        break;
      case FORWARD:
        if (minimalIndices_.getSize() == 0)
          throw InvalidArgumentException(HERE) << "Error: minimal indices must be specified when direction is FORWARD";
        buildCurrentMatrixFromIndices(minimalIndices_);
        for (UnsignedInteger i = 0; i < minimalIndices_.getSize(); ++i)
          columnMaxToCurrent[minimalIndices_[i]] = i;
        break;
      case BOTH:
        throw InvalidArgumentException(HERE) << "Error: start indices must be specified when direction is BOTH";
        break;
    }
  }
  else
  {
    buildCurrentMatrixFromIndices(startIndices_);
    for (UnsignedInteger i = 0; i < startIndices_.getSize(); ++i)
      columnMaxToCurrent[minimalIndices_[i]] = i;
  }

  UnsignedInteger iterations = 1;
  NumericalScalar Lstar;
  while(iterations < maximumIterationNumber_)
  {
    // Update A=(X^T*X)^{-1}, B = X^T*Y, residual = Y - X*A*X^T*Y
    Lstar = penalty_ * currentX_.getNbColumns() - 2.0 * computeLogLikelihood();
    LOGDEBUG(OSS() << "Iteration " << iterations << ", current criterion=" << Lstar);

    NumericalScalar LF = SpecFunc::MaxNumericalScalar;
    UnsignedInteger indexF = maxX_.getNbColumns();
    if (direction_ == FORWARD || direction_ == BOTH)
    {
      // M = X * A
      const Matrix M(currentX_ * currentGramInverse_);
      // indexSet = Imax - I*
      Indices indexSet;
      for (UnsignedInteger i = 0; i < maxX_.getNbColumns(); ++i)
      {
        if (!currentIndices_.contains(i))
          indexSet.add(i);
      }
      UpdateForwardFunctor updateFunctor(indexSet, currentX_, maxX_, currentResidual_, M);
      TBB::ParallelReduce(0, indexSet.getSize(), updateFunctor);
      indexF = updateFunctor.bestIndex_;
      LF = penalty_ * (currentX_.getNbColumns() + 1) + size * (1.0 + std::log(2.0 * M_PI) + std::log(updateFunctor.criterion_ / size));
      LOGDEBUG(OSS() << "Best candidate in forward direction is " << indexF << ", squared residual norm=" << updateFunctor.criterion_ << ", criterion=" << LF);
    }
    NumericalScalar LB = SpecFunc::MaxNumericalScalar;
    UnsignedInteger indexB = maxX_.getNbColumns();
    if (direction_ == BACKWARD || direction_ == BOTH)
    {
      // indexSet = I* - Imin
      Indices indexSet;
      for (UnsignedInteger i = 0; i < currentIndices_.getSize(); ++i)
      {
        if (!minimalIndices_.contains(currentIndices_[i]))
          indexSet.add(currentIndices_[i]);
      }
      UpdateBackwardFunctor updateFunctor(indexSet, columnMaxToCurrent, currentIndices_, currentX_, Y_, currentGramInverse_, currentB_);
      TBB::ParallelReduce(0, indexSet.getSize(), updateFunctor);
      indexB = updateFunctor.bestIndex_;
      LB = penalty_ * (currentX_.getNbColumns() - 1) + size * (1.0 + std::log(2.0 * M_PI) + std::log(updateFunctor.criterion_ / size));
      LOGDEBUG(OSS() << "Best candidate in backward direction is " << indexB << ", squared residual norm=" << updateFunctor.criterion_ << ", criterion=" << LB);
    }
    if (!(LF < Lstar || LB < Lstar))
      break;

    if (LF < LB)
    {
      LOGDEBUG(OSS() << "Add column " << indexF);
      // Add indexF to currentIndices_
      currentIndices_.add(indexF);
      // Add column indexF to currentX_
      columnMaxToCurrent[indexF] = currentX_.getNbColumns();
      Matrix newX(currentX_.getNbRows(), currentX_.getNbColumns() + 1);
      memcpy(&newX(0, 0), &currentX_(0, 0), sizeof(NumericalScalar)*size*currentX_.getNbColumns());
      memcpy(&newX(0, currentX_.getNbColumns()), &maxX_(0, indexF), sizeof(NumericalScalar)*size);
      currentX_ = newX;
    }
    else
    {
      LOGDEBUG(OSS() << "Remove column " << indexB);
      // Remove column indexB from currentX_
      Matrix newX(currentX_.getNbRows(), currentX_.getNbColumns() - 1);
      const UnsignedInteger pos(columnMaxToCurrent[indexB]);
      memcpy(&newX(0, 0), &currentX_(0, 0), sizeof(NumericalScalar)*size*pos);
      if (pos+1 != currentX_.getNbColumns())
        memcpy(&newX(0, pos), &currentX_(0, pos+1), sizeof(NumericalScalar)*size*(currentX_.getNbColumns() - pos - 1));
      currentX_ = newX;
      // Update columnMaxToCurrent
      for (Indices::iterator it = columnMaxToCurrent.begin(); it != columnMaxToCurrent.end(); ++it)
      {
        if (*it > indexB && *it != maxX_.getNbColumns())
          --(*it);
      }
      columnMaxToCurrent[indexB] = maxX_.getNbColumns();
      // Remove indexB from currentIndices_
      Indices newIndices;
      for (Indices::const_iterator it = currentIndices_.begin(); it != currentIndices_.end(); ++it)
      {
        if (*it != indexB)
          newIndices.add(*it);
      }
      currentIndices_ = newIndices;
    }
    LOGDEBUG(OSS() << "Index set is now " << currentIndices_.__str__());
    ++iterations;
  }
  // Update A=(X^T*X)^{-1}, B = X^T*Y, residual = Y - X*A*X^T*Y
  const UnsignedInteger p(currentX_.getNbColumns());
  const NumericalScalar criterion(penalty_ * p - 2.0 * computeLogLikelihood());
  LOGDEBUG(OSS() << "Final indices are " << currentIndices_.__str__() << " and criterion is " << criterion);
  NumericalPoint regression(p);
  memcpy(&regression[0], &currentB_(0, 0), sizeof(NumericalScalar)*p);
  NumericalPoint residual(size);
  memcpy(&residual[0], &currentResidual_(0, 0), sizeof(NumericalScalar)*size);
  // result_ = LinearModelResult(inputSample_, outputSample_, LinearModel(regression), residual, residual);
  hasRun_ = true;
}

/* Build currentX_ and currentIndices_ from given indices */
void LinearModelStepwiseAlgorithm::buildCurrentMatrixFromIndices(const Indices & columns)
{
  const UnsignedInteger size(inputSample_.getSize());
  currentX_ = Matrix(size, columns.getSize());
  currentIndices_ = columns;
  for (UnsignedInteger i = 0; i < columns.getSize(); ++i)
    memcpy(&currentX_(0, columns[i]), &maxX_(0, i), sizeof(NumericalScalar)*size);
}


/* Get linear model result */
LinearModelResult LinearModelStepwiseAlgorithm::getResult()
{
  if (!hasRun_) run();
  return result_;
}

/* Compute the likelihood function */
NumericalScalar LinearModelStepwiseAlgorithm::computeLogLikelihood()
{
  CovarianceMatrix XtX(currentX_.computeGram(true));
  currentGramInverse_ = XtX.solveLinearSystem(IdentityMatrix(XtX.getNbRows()), false);
  // B = X^T * Y
  currentB_ = currentX_.getImplementation()->genProd(*(Y_.getImplementation()), true, false);
  // M = X * A
  const Matrix M(currentX_ * currentGramInverse_);
  // residual = Y - X*A*X^t*Y
  const Matrix Yhat(M * currentB_);
  currentResidual_ = Y_ - Yhat;
  const UnsignedInteger size(currentResidual_.getNbRows());
  NumericalPoint residualNP(size, 0.0);
  memcpy(&residualNP[0], &currentResidual_(0,0), sizeof(NumericalScalar) * size);

  const NumericalScalar normSquared = residualNP.normSquare();
  const NumericalScalar result = -(0.5 * size) * (1.0 + std::log(2.0 * M_PI) + std::log(normSquared / size));
  LOGDEBUG(OSS() << "Residual squared norm=" << normSquared << ", loglikelihood=" << result);
  return result;
}

/* Method save() stores the object through the StorageManager */
void LinearModelStepwiseAlgorithm::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  throw NotYetImplementedException(HERE);
}

/* Method load() reloads the object from the StorageManager */
void LinearModelStepwiseAlgorithm::load(Advocate & adv)
{
  PersistentObject::load(adv);
  throw NotYetImplementedException(HERE);
}



END_NAMESPACE_OPENTURNS

