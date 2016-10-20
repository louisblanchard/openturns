//                                               -*- C++ -*-
/**
 *  @brief The linear model analysis
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
#ifndef OPENTURNS_LINEARMODELANALYSIS_HXX
#define OPENTURNS_LINEARMODELANALYSIS_HXX

#include "openturns/PersistentObject.hxx"
#include "openturns/LinearModelResult.hxx"
#include "openturns/NumericalSample.hxx"
#include "openturns/Description.hxx"
#include "openturns/TestResult.hxx"
#include "openturns/Graph.hxx"


BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LinearModelAnalysis
 *
 * The linear model analysis
 */

class OT_API LinearModelAnalysis : 
  public PersistentObject
{
  CLASSNAME;

public:

  /** Default constructor */
  LinearModelAnalysis();

  /** Parameter constructor */
  LinearModelAnalysis(const LinearModelResult & linearModelResult);

  /** Virtual constructor */
  LinearModelAnalysis * clone() const;

  /** String converter */
  String __repr__() const;

  /** Method that returns the ANOVA table (ANalyse Of VAriance) */
  String __str__() const;

  /** Linear model accessor */
  LinearModelResult getLinearModelResult() const;

  /** Accessors to data from ANOVA table */
  String getFormula() const; 
  Description getCoefficientsNames() const;
  NumericalSample getResiduals() const;
  NumericalSample getStandardizedResiduals() const;
  NumericalSample getCoefficientsEstimates() const;
  NumericalSample getCoefficientsStandardErrors() const;
  NumericalSample getCoefficientsTScores() const;
  NumericalSample getCoefficientsPValues() const;

  /**  leverages */
  NumericalPoint getLeverages() const;

  /**  Cook's distances */
  NumericalPoint getCookDistances() const;

  /** Number of degrees of freedom */
  UnsignedInteger getDegreesOfFreedom() const;

  /** R-squared */
  NumericalScalar getRSquared() const;

  /** Adjusted R-squared */
  NumericalScalar getAdjustedRSquared() const;

  /** Fisher test */
  NumericalScalar getFisherScore() const;
  NumericalScalar getFisherPValue() const;

  /** Kolmogorov-Smirnov normality test */
  TestResult getNormalityTestResultKolmogorovSmirnov() const;

  /** Anderson-Darling normality test */
  TestResult getNormalityTestResultAndersonDarling() const;

  /** Chi-Squared normality test */
  TestResult getNormalityTestResultChiSquared() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

  /** [1] Draw a plot of residuals versus fitted values  */
  Graph drawResidualsVsFitted() const;

  /** [2] a Scale-Location plot of sqrt(| residuals |) versus fitted values */
  Graph drawScaleLocation() const;

  /** [3] a Normal quantiles-quantiles plot of standardized residuals */
  Graph drawQQplot() const;

  /** [4] a plot of Cook's distances versus row labels */
  Graph drawCookDistance() const;

  /** [5] a plot of residuals versus leverages that adds bands corresponding to Cook's distances of 0.5 and 1. */
  Graph drawResidualsVsLeverages() const;

  /** [6] a plot of Cook's distances versus leverage/(1-leverage) */
  Graph drawCookVsLeverages() const;

private:

  /** This class is in a module, add ResourceMap keys in this function */
  static void InitializeResourceMap();

  /** linear model result */
  LinearModelResult linearModelResult_;

} ; /* class LinearModelAnalysis */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARMODELANALYSIS_HXX */