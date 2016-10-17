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

  /** Parameter constructor */
  LinearModelResult(const NumericalSample & inputSample,
                    const NumericalSample & outputSample,
                    const LinearModel & model,
                    const NumericalPoint & residuals,
                    const NumericalPoint & relativeErrors);

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

  /** set Condensed formula */ 
  void setFormula(const String formula);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** input data */
  NumericalSample inputSample_;

  /** output data */
  NumericalSample outputSample_;

  /** linear model */
  LinearModel linearModel_;

  /** Cook's distances */
  NumericalSample CookDistances_;

  /** The formula description */
  String condensedFormula_;


} ; /* class LinearModelResult */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARMODELRESULT_HXX */
