//                                               -*- C++ -*-
/**
 *  @brief This is the orthogonal function basis
 *
 *  Copyright 2005-2017 Airbus-EDF-IMACS-Phimeca
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
#include "openturns/OrthogonalProductFunctionFactory.hxx"
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



TEMPLATE_CLASSNAMEINIT(PersistentCollection<OrthogonalUniVariateFunctionFamily>);

static const Factory<PersistentCollection<OrthogonalUniVariateFunctionFamily> > Factory_PersistentCollection_OrthogonalUniVariateFunctionFamily;

CLASSNAMEINIT(OrthogonalProductFunctionFactory);

static const Factory<OrthogonalProductFunctionFactory> Factory_OrthogonalProductFunctionFactory;

typedef Collection<NumericalPoint> NumericalPointCollection;
typedef ProductUniVariateFunctionEvaluationImplementation::UniVariateFunctionCollection UniVariateFunctionCollection;

/* Default constructor */
OrthogonalProductFunctionFactory::OrthogonalProductFunctionFactory()
  : OrthogonalFunctionFactory()
  , productFunctionFactory_()
{
  // Nothing to do
}


/* Constructor */
OrthogonalProductFunctionFactory::OrthogonalProductFunctionFactory(const FunctionFamilyCollection & coll)
  : OrthogonalFunctionFactory()
{
  buildProductFunctionFactory(coll);
  buildMeasure(coll);
}


/* Constructor */
OrthogonalProductFunctionFactory::OrthogonalProductFunctionFactory(const FunctionFamilyCollection & coll,
    const EnumerateFunction & phi)
  : OrthogonalFunctionFactory()
{
  if (coll.getSize() != phi.getDimension()) throw InvalidArgumentException(HERE) << "Error: the enumerate function must have a dimension equal to the collection size";
  buildProductFunctionFactory(coll); 
  buildMeasure(coll);
}


/* Virtual constructor */
OrthogonalProductFunctionFactory * OrthogonalProductFunctionFactory::clone() const
{
  return new OrthogonalProductFunctionFactory(*this);
}


/* Return the enumerate function that translate unidimensional indices into multidimensional indices */
EnumerateFunction OrthogonalProductFunctionFactory::getEnumerateFunction() const
{
  return productFunctionFactory_.getEnumerateFunction();
}

/* Build the NumericalMathFunction of the given index */
NumericalMathFunction OrthogonalProductFunctionFactory::build(const UnsignedInteger index) const
{
  return productFunctionFactory_.build(index);
}


/* String converter */
String OrthogonalProductFunctionFactory::__repr__() const
{
  return OSS() << "class=" << getClassName()
         << " product Function Factory=" << productFunctionFactory_
         << " measure=" << measure_;
}


/* Method save() stores the object through the StorageManager */
void OrthogonalProductFunctionFactory::save(Advocate & adv) const
{
  OrthogonalFunctionFactory::save(adv);
  adv.saveAttribute( "productFunctionFactory_", productFunctionFactory_ );
}


/* Method load() reloads the object from the StorageManager */
void OrthogonalProductFunctionFactory::load(Advocate & adv)
{
  OrthogonalFunctionFactory::load(adv);
  adv.loadAttribute( "productFunctionFactory_", productFunctionFactory_ );
}

/*  Build product function factory */
void OrthogonalProductFunctionFactory::buildProductFunctionFactory(const FunctionFamilyCollection & coll)
{
  ProductFunctionFactory::FunctionFamilyCollection functionColl;
  const UnsignedInteger size = coll.getSize();
  for (UnsignedInteger i = 0; i < size; ++ i)
  {
    functionColl.add(UniVariateFunctionFamily(coll[i].getImplementation()));
  }
  //productFunctionFactory_()
}

/* Build the measure based on the one found in the family collection */
void OrthogonalProductFunctionFactory::buildMeasure(const FunctionFamilyCollection & coll)
{
  const UnsignedInteger size = coll.getSize();
  Collection<Distribution> distributions(size);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    distributions[i] = coll[i].getMeasure();
  }
  measure_ = ComposedDistribution(distributions);
}


END_NAMESPACE_OPENTURNS
