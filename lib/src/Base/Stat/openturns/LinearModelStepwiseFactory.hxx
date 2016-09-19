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
public:

  /** Default constructor is private */
  LinearModelStepwiseFactory();

  /** Parameters constructor */
  explicit LinearModelStepwiseFactory(const NumericalSample & inputSample);

  /** Virtual constructor */
  virtual LinearModelStepwiseFactory * clone() const;

  /** Sample accessor */
  NumericalSample getInputSample() const;

  /** formula accessor */
  String getFormula() const;

  /** Interactions accessor */
  Description getInteractions(const Description & monomials, const UnsignedInteger degree) const;

  /** Polynomial accessor */
  Description getPolynomial(const Description & monomial, const UnsignedInteger degree) const;
  String getPolynomial(const String & monomial, const UnsignedInteger degree) const;

  /** columns Indices accessor */
  Indices getIndices(const Description & indice) const;

  /** add methods */
  void add(const Description & name);
  void add(const String & name);

  /** Build a linear model using stepwise regression with "forward" search method */
  LinearModelResult buildForward(Indices Smin, 
                                 Indices Smax, 
                                 const NumericalScalar k); 

  /** Build a linear model using stepwise regression with "backward" search method */
  LinearModelResult buildBackward(Indices Smax, 
                                  Indices Smin, 
                                  const NumericalScalar k); 

  /** Build a linear model using stepwise regression with "both" search method */
  LinearModelResult buildBoth(Indices Smin, 
                              Indices Smax, 
                              Indices Sini, 
                              const NumericalScalar k); 


private:

  /** The input data  */ 
  NumericalSample inputSample_;

  /** The indices of minimal model */ 
  Indices Smin_;

  /** The indices of maximal model */ 
  Indices Smax_;

  /** The formula description */
  String formula_;

  /** The monomials collection */ 
  String monomial_;

  /** The current matrix */ 
  Matrix Zcurrent_; 

  /** The function */
  NumericalMathFunction func_;

  /** Build a linear model using stepwise regression */
  LinearModelResult build(Indices Smin, 
                          Indices Smax, 
                          Indices Sini,
                          const NumericalScalar k);

  /** functions to find argmax of the optimal criteria  */ 
  NumericalScalar evaluateWith(const Indices j);
  NumericalScalar evaluateWithout(const Indices j);  

}; /* class LinearModelStepwiseFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARMODELSTEPWISEFACTORY_HXX */
