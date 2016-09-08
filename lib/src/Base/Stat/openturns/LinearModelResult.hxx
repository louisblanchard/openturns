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

#include "openturns/LinearModel.hxx"
#include "openturns/PersistentObject.hxx"
#include "openturns/NumericalSample.hxx"
#include "openturns/Graph.hxx"


BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LinearModelResult
 *
 * The result of a linear model evaluation
 */

class OT_API LinearModelResult
  : public PersistentObject 
{
  CLASSNAME;

public:


  /** Default constructor */
  LinearModelResult();

  /** Parameter constructor */
  LinearModelResult(const NumericalSample & inputData,
                    const NumericalSample & outputData,
                    const LinearModel & model);

  /** Virtual constructor */
  LinearModelResult * clone() const;

  /** String converter */
  String __repr__() const;

  /** Linear model accessor */
  LinearModel getLinearModel() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


protected:

  /** Method that returns the ANOVA table (ANalyse Of VAriance) */
  void printANOVAtable() const;

  /** [1] Draw a plot of residuals versus fitted values  */
  Graph drawResidualsVsFitted() const;

  /** [2] a Scale-Location plot of sqrt(| residuals |) versus fitted values */
  Graph drawScaleLocation() const;

  /** [3] a Normal quantiles-quantiles plot of standardized residuals */
  Graph drawNormalQQ() const;

  /** [4] a plot of Cook's distances versus row labels */
  Graph drawCookDistanceVsLabels() const;

  /** [5] a plot of residuals versus leverages that adds bands corresponding to Cook's distances of 0.5 and 1. */
  Graph drawResidualsVsLeverages() const;

  /** [6] a plot of Cook's distances versus leverage/(1-leverage) */
  Graph drawCookDistanceVsLeverages() const;

private:

  /** inputData should be keeped*/
  NumericalSample inputData_;

  /** The linear model */
  LinearModel linearModel_;


} ; /* class LinearModelResult */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARMODELRESULT_HXX */
