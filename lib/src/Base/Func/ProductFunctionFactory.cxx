//                                               -*- C++ -*-
/**
 *  @brief This is the  function basis
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
#include "openturns/ProductFunctionFactory.hxx"
#include "openturns/OSS.hxx"
#include "openturns/PersistentObjectFactory.hxx"
#include "openturns/Exception.hxx"
#include "openturns/Indices.hxx"
#include "openturns/ComposedDistribution.hxx"
#include "openturns/NumericalMathFunctionImplementation.hxx"
#include "openturns/ProductUniVariateFunctionEvaluationImplementation.hxx"
#include "openturns/ProductUniVariateFunctionGradientImplementation.hxx"
#include "openturns/ProductUniVariateFunctionHessianImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



TEMPLATE_CLASSNAMEINIT(PersistentCollection<UniVariateFunctionFamily>);

static const Factory<PersistentCollection<UniVariateFunctionFamily> > Factory_PersistentCollection_UniVariateFunctionFamily;

CLASSNAMEINIT(ProductFunctionFactory);

static const Factory<ProductFunctionFactory> Factory_ProductFunctionFactory;

typedef Collection<NumericalPoint> NumericalPointCollection;
typedef ProductUniVariateFunctionEvaluationImplementation::UniVariateFunctionCollection UniVariateFunctionCollection;

/* Default constructor */
ProductFunctionFactory::ProductFunctionFactory()
  : FunctionalBasisImplementation()
  , coll_()
  , phi_()
{
  // Nothing to do
}


/* Constructor */
ProductFunctionFactory::ProductFunctionFactory(const FunctionFamilyCollection & coll)
  : FunctionalBasisImplementation()
  , coll_(coll)
  , phi_(coll.getSize())
{
  // Nothing to do
}


/* Constructor */
ProductFunctionFactory::ProductFunctionFactory(const FunctionFamilyCollection & coll,
    const EnumerateFunction & phi)
  : FunctionalBasisImplementation(),
    coll_(coll),
    phi_(phi)
{
  if (coll.getSize() != phi.getDimension()) throw InvalidArgumentException(HERE) << "Error: the enumerate function must have a dimension equal to the collection size";
}


/* Virtual constructor */
ProductFunctionFactory * ProductFunctionFactory::clone() const
{
  return new ProductFunctionFactory(*this);
}


/* Return the enumerate function that translate unidimensional indices into multidimensional indices */
EnumerateFunction ProductFunctionFactory::getEnumerateFunction() const
{
  return phi_;
}

/* set the enumerate function that translate unidimensional indices into multidimensional indices */
void ProductFunctionFactory::setEnumerateFunction(const EnumerateFunction & phi) 
{
  phi_ = phi;
}

/* set the function familly collection */
void ProductFunctionFactory::setFunctionFamilyCollection(const FunctionFamilyCollection & coll)
{
  coll_ = coll;
}

/* Build the NumericalMathFunction of the given index */
NumericalMathFunction ProductFunctionFactory::build(const UnsignedInteger index) const
{
  // Compute the multi-indices using the EnumerateFunction
  Indices indices(phi_(index));
  const UnsignedInteger size = indices.getSize();
  // Then build the collection of functions using the collection of factories
  UniVariateFunctionCollection functions(size);
  for (UnsignedInteger i = 0; i < size; ++ i)
  {
    functions[i] = coll_[i].build(indices[i]);
  }
  const Pointer<ProductUniVariateFunctionEvaluationImplementation> p_evaluation(ProductUniVariateFunctionEvaluationImplementation(functions).clone());
  return NumericalMathFunctionImplementation(p_evaluation,
         ProductUniVariateFunctionGradientImplementation(p_evaluation).clone(),
         ProductUniVariateFunctionHessianImplementation(p_evaluation).clone());
}


/* String converter */
String ProductFunctionFactory::__repr__() const
{
  return OSS() << "class=" << getClassName()
         << " univariate function collection=" << coll_;
}


/* Method save() stores the object through the StorageManager */
void ProductFunctionFactory::save(Advocate & adv) const
{
  FunctionalBasisImplementation::save(adv);
  adv.saveAttribute( "coll_", coll_ );
  adv.saveAttribute( "phi_", phi_ );
}


/* Method load() reloads the object from the StorageManager */
void ProductFunctionFactory::load(Advocate & adv)
{
  FunctionalBasisImplementation::load(adv);
  adv.loadAttribute( "coll_", coll_ );
  adv.loadAttribute( "phi_", phi_ );
}


END_NAMESPACE_OPENTURNS
