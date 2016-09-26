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
#ifndef OPENTURNS_LINEARMODELSTEPWISEFACTORY_HXX
#define OPENTURNS_LINEARMODELSTEPWISEFACTORY_HXX

#include "openturns/PersistentObject.hxx"
#include "openturns/Description.hxx"
#include "openturns/LinearModelResult.hxx"
#include "openturns/NumericalSample.hxx"
#include "openturns/NumericalMathFunction.hxx"
#include "openturns/Matrix.hxx"
#include "openturns/CovarianceMatrix.hxx"
#include "openturns/ResourceMap.hxx"
#include "openturns/Indices.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LinearModelStepwiseFactory
 *
 * Factory for linear model using stepwise regression
 */

class OT_API LinearModelStepwiseFactory :
  public PersistentObject
{
  CLASSNAME;

public:

  enum Direction { FORWARD = 1, BACKWARD = -1, BOTH = 0 };

  /** Default constructor is private */
  LinearModelStepwiseFactory();

  /** Parameters constructor */
  explicit LinearModelStepwiseFactory(const Description & variables,
                                      const Direction direction,
                                      const NumericalScalar penalty,
                                      const UnsignedInteger maximumIterationNumber);

  /** Virtual constructor */
  virtual LinearModelStepwiseFactory * clone() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Get Formula */
  String getFormula() const;

  /** Get direction of the stepwise regression method  */
  Direction getDirection() const;

  /** Get penalty of the stepwise regression method */
  NumericalScalar getPenalty() const;

  /** Set direction of the stepwise regression method */
  void setDirection(const Direction direction);

  /** Set penalty of the stepwise regression method  */
  void setPenalty(const NumericalScalar penalty);

  /** Set maximum number of iterations of the stepwise regression method  */
  void setMaximumIterationNumber(const NumericalScalar maxiter);

  /** Add formulas */
  void add(const Description & formulas);
  void add(const String & formula);

  /** Get column indices of given formulas */
  Indices getIndices(const Description & formulas) const;

  /** Get formulas of interactions between variables */
  Description getInteractions(const UnsignedInteger degree, const Description & variables = Description()) const;

  /** Get formulas of monomials */
  Description getPolynomial(const UnsignedInteger degree, const Description & variables = Description()) const;
  String getPolynomial(const UnsignedInteger degree, const String & variable) const;

  /** Build a linear model using stepwise regression with "forward" search method */
  LinearModelResult build(const NumericalSample & inputSample,
                          const NumericalSample & outputSample,
                          const Indices & minimalIndices,
                          const Indices & startIndices);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);



private:

  /** Input variables */
  Description variables_;

  /** The input data  */
  NumericalSample inputSample_;

  /** The output data  */
  NumericalSample outputSample_;

  /** The formula description */
  String condensedFormula_;

  /** The monomials collection */
  Description formulas_;

  /** The multiple of the degrees of freedom used for the penalty of the stepwise regression method
      - 2      Akaike   information criterion (AIC)
      - log(n) Bayesian information criterion (BIC)  */
  NumericalScalar penalty_;

  /** The maximum number of iterations of the stepwise regression method */
  NumericalScalar maxiter_; 

  /** The direction of the stepwise regression method */
  Direction direction_;

  /** The matrix X^T_{max} containing all monomials */
  Matrix maxXt_;

  /** The current matrix */
  Matrix currentXt_;

  /** The covariance matrix inverse: A=(X^T X)^-1 */
  CovarianceMatrix currentGramInverse_;

  /** The current B NumericalPoint */
  NumericalPoint currentB_;

  /** The current M matrix */
  Matrix currentM_;

  /** The current M*B NumericalPoint */
  NumericalPoint currentMB_;

  /** The indices of current model */
  Indices currentIndices_;

  /** Compute the likelihood function */
  NumericalScalar computeLogLikelihood();  
  
  /** Compute the likelihood function for stepwise regression with "forward" search method */
  NumericalScalar computeLogLikelihoodForward(const UnsignedInteger index);

  /** Compute the likelihood function for stepwise regression with "backward" search method */
  NumericalScalar computeLogLikelihoodBackward(const UnsignedInteger index);

}; /* class LinearModelStepwiseFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARMODELSTEPWISEFACTORY_HXX */
