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
#ifndef OPENTURNS_LINEARMODELRESULT_HXX
#define OPENTURNS_LINEARMODELRESULT_HXX

#include "openturns/MetaModelResult.hxx"
#include "openturns/LinearModel.hxx"
#include "openturns/NumericalSample.hxx"
#include "openturns/DesignProxy.hxx"


BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LinearModelResult
 *
 * The result of a linear model evaluation
 */

class OT_API LinearModelResult
  : public MetaModelResult
{
  CLASSNAME;

public:


  /** Default constructor */
  LinearModelResult();

  /* We cannot use a DesignProxy here since it is not persistent */
  /** Parameter constructor */
  LinearModelResult(const NumericalSample & inputSample,
                    const Basis & basis,
                    const Matrix & design,
                    const NumericalSample & outputSample,
                    const LinearModel & model,
                    const String & formula,
                    const Description & coefficientsNames,
                    const NumericalSample & sampleResiduals,
                    const NumericalPoint & diagonalA,
                    const NumericalPoint & leverages,
                    const NumericalPoint & cookDistances);

  /** Virtual constructor */
  LinearModelResult * clone() const;

  /** String converter */
  String __repr__() const;

  /** Sample accessors */
  NumericalSample getInputSample() const;
  NumericalSample getOutputSample() const;

  /** Linear model accessor */
  LinearModel getLinearModel() const;

  /** Condensed formula accessor */
  String getFormula() const;

  /** Coefficients names accessor */
  Description getCoefficientsNames() const;

  /** Residuals accessor */
  NumericalSample getSampleResiduals() const;

  /** Standardized residuals accessor */
  NumericalSample getStandardizedResiduals() const;

  /** Leverages accessor */
  NumericalPoint getLeverages() const;

  /** Cook distance accessor */
  NumericalPoint getCookDistances() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** input data */
  NumericalSample inputSample_;

  /** basis */
  Basis basis_;

  /** input data */
  Matrix design_;

  /** output data */
  NumericalSample outputSample_;

  /** linear model */
  LinearModel linearModel_;

  /** The formula description */
  String condensedFormula_;

  /** Coefficients names */
  Description coefficientsNames_;

  /** Whole residuals */
  NumericalSample sampleResiduals_;

  /** Standardized residuals */
  NumericalSample standardizedResiduals_;

  /** Diagonal of (Xt X)^{-1} */
  NumericalPoint diagonalA_;

  /** Leverages */
  NumericalPoint leverages_;

  /** Cook's distances */
  NumericalPoint cookDistances_;


} ; /* class LinearModelResult */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARMODELRESULT_HXX */
