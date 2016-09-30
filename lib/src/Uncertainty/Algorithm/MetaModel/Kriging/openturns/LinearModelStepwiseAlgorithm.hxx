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
#include "openturns/Matrix.hxx"
#include "openturns/CovarianceMatrix.hxx"
#include "openturns/Indices.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LinearModelStepwiseAlgorithm
 *
 * The class building linear model using stepwise regression
 */

class OT_API LinearModelStepwiseAlgorithm :
  public PersistentObject
{
  CLASSNAME;

public:

  enum Direction { BACKWARD = -1, BOTH = 0, FORWARD = 1 };

  /** Default constructor */
  LinearModelStepwiseAlgorithm();

  /** Parameters constructor */
  LinearModelStepwiseAlgorithm(const NumericalSample & inputSample,
                               const NumericalSample & outputSample,
                               const SignedInteger direction = BOTH,
                               const NumericalScalar penalty = -1.0 /* < 0 means BIC, by convention */,
                               const UnsignedInteger maximumIterationNumber = 1000);

  /** Virtual constructor */
  virtual LinearModelStepwiseAlgorithm * clone() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Sample accessors */
  NumericalSample getInputSample() const;
  NumericalSample getOutputSample() const;

  /** Direction accessors */
  Direction getDirection() const;
  void setDirection(const SignedInteger direction);

  /** Penalty accessors */
  NumericalScalar getPenalty() const;
  void setPenalty(const NumericalScalar penalty);

  /** Maximum number of iterations accessors */
  UnsignedInteger getMaximumIterationNumber() const;
  void setMaximumIterationNumber(const UnsignedInteger maximumIterationNumber);

  /** Condensed formula accessor */
  String getFormula() const;

  /** Add formulas */
  void add(const Description & formulas);
  void add(const String & formula);
  void add(const NumericalSample & userColumns);

  /** Remove formulas/columns */
  void remove(const Description & formulas);
  void remove(const Indices & indices);

  /** Get column indices of given formulas */
  Indices getIndices(const Description & formulas) const;

  /** Interactions between variables*/
  Description getInteractions(const UnsignedInteger degree, const Description & variables = Description()) const;
  void addInteractions(const UnsignedInteger degree, const Description & variables = Description());
  void removeInteractions(const UnsignedInteger degree, const Description & variables = Description());

  /** Power of variables */
  Description getPower(const UnsignedInteger degree, const Description & variables = Description()) const;
  void addPower(const UnsignedInteger degree, const Description & variables = Description());
  void removePower(const UnsignedInteger degree, const Description & variables = Description());

  /** Set indices of minimal model */
  void setMinimalIndices(const Indices & minimalIndices);

  /** Set indices of start model */
  void setStartIndices(const Indices & startIndices);

  /** Perform regression */
  void run();

  /** Result accessor */
  LinearModelResult getResult();

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** Compute the likelihood function */
  NumericalScalar computeLogLikelihood();

  /** Build currentX_ and currentIndices_ from given indices */
  void buildCurrentMatrixFromIndices(const Indices & columns);

  /** Input sample */
  NumericalSample inputSample_;

  /** Output sample */
  NumericalSample outputSample_;

  /** The direction of the stepwise regression method */
  Direction direction_;

  /** The multiple of the degrees of freedom used for the penalty of the stepwise regression method
      - 2      Akaike   information criterion (AIC)
      - log(n) Bayesian information criterion (BIC)  */
  NumericalScalar penalty_;

  /** The maximum number of iterations of the stepwise regression method */
  NumericalScalar maximumIterationNumber_;

  /** The indices of minimal model */
  Indices minimalIndices_;

  /** The indices of start model */
  Indices startIndices_;

  /** The formula description */
  String condensedFormula_;

  /** The monomials collection */
  Description formulas_;

  /** User-defined columns */
  NumericalSample userColumns_;

  /** The output data, stored as a matrix  */
  Matrix Y_;

  /** The matrix X_{max} containing all monomials */
  Matrix maxX_;

  /** The current matrix */
  Matrix currentX_;

  /** The covariance matrix inverse: A=(X^T X)^-1 */
  CovarianceMatrix currentGramInverse_;

  /** The current matrix B=X^T Y */
  Matrix currentB_;

  /** The indices of current model */
  Indices currentIndices_;

  /** The position of currentIndices_ columns in currentX_ */
  Indices columnCurrentX_;

  /** Result */
  LinearModelResult result_;

  /** Whether result is cached */
  Bool hasRun_;
}; /* class LinearModelStepwiseAlgorithm */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARMODELSTEPWISEFACTORY_HXX */
