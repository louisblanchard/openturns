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
#include "openturns/LinearModelResult.hxx"
#include "openturns/NumericalSample.hxx"
#include "openturns/Matrix.hxx"
#include "openturns/Indices.hxx"
#include "openturns/Basis.hxx"

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

  /** Parameters constructor FORWARD and BACKWARD */
  LinearModelStepwiseAlgorithm(const NumericalSample & inputSample,
                               const Basis & basis,
                               const NumericalSample & outputSample,
                               const Indices & minimalIndices,
                               const Bool isForward = true,
                               const NumericalScalar penalty = -1.0 /* < 0 means BIC, by convention */,
                               const UnsignedInteger maximumIterationNumber = 1000);

  /** Parameters constructor BOTH */
  LinearModelStepwiseAlgorithm(const NumericalSample & inputSample,
                               const Basis & basis,
                               const NumericalSample & outputSample,
                               const Indices & minimalIndices,
                               const Indices & startIndices,
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

  /** Penalty accessors */
  NumericalScalar getPenalty() const;

  /** Maximum number of iterations accessors */
  UnsignedInteger getMaximumIterationNumber() const;

  /** Condensed formula accessor */
  String getFormula() const;

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

  /** The monomials collection */
  Basis basis_;

  /** Output sample */
  NumericalSample outputSample_;

  /** The direction of the stepwise regression method */
  Direction direction_;

  /** The multiple of the degrees of freedom used for the penalty of the stepwise regression method
      - 2      Akaike   information criterion (AIC)
      - log(n) Bayesian information criterion (BIC)  */
  NumericalScalar penalty_;

  /** The maximum number of iterations of the stepwise regression method */
  UnsignedInteger maximumIterationNumber_;

  /** The indices of minimal model */
  Indices minimalIndices_;

  /** The indices of start model */
  Indices startIndices_;

  /** The formula description */
  String condensedFormula_;

  /** The output data, stored as a matrix  */
  Matrix Y_;

  /** The matrix X_{max} containing all monomials */
  Matrix maxX_;

  /** The current matrix */
  Matrix currentX_;

  /** QR decomposition of the current matrix */
  Matrix currentQ_;
  Matrix currentInvRt_;

  /** The current residual R=Y - Q Q^T Y */
  Matrix currentResidual_;

  /** The indices of current model */
  Indices currentIndices_;

  /** Result */
  LinearModelResult result_;

  /** Whether result is cached */
  Bool hasRun_;
}; /* class LinearModelStepwiseAlgorithm */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARMODELSTEPWISEFACTORY_HXX */
