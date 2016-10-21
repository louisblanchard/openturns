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
#include "openturns/LinearModelAnalysis.hxx"
#include "openturns/PersistentObjectFactory.hxx"
#include "openturns/NormalityTest.hxx"
#include "openturns/OSS.hxx"
#include "openturns/ResourceMap.hxx"
#include "openturns/Cloud.hxx"
#include "openturns/Curve.hxx"
#include "openturns/Text.hxx"
#include "openturns/Normal.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(LinearModelAnalysis);

static const Factory<LinearModelAnalysis> Factory_LinearModelAnalysis;

/* Default constructor */
LinearModelAnalysis::LinearModelAnalysis()
  : PersistentObject()
{
  InitializeResourceMap();
}

/* Parameter constructor */
LinearModelAnalysis::LinearModelAnalysis(const LinearModelResult & linearModelResult)
  : PersistentObject()
  , linearModelResult_(linearModelResult)
{
  InitializeResourceMap();
}

/* Virtual constructor */
LinearModelAnalysis * LinearModelAnalysis::clone() const
{
  InitializeResourceMap();
  return new LinearModelAnalysis(*this);
}


/* String converter */
String LinearModelAnalysis::__repr__() const
{
  return OSS(true)
         << "class=" << getClassName()
         << "linearModelResult=" << linearModelResult_;
}


/* Method that returns the ANOVA table (ANalyse Of VAriance) */
String LinearModelAnalysis::__str__() const
{
  throw NotYetImplementedException(HERE);
}


/* Linear model accessor */
LinearModelResult LinearModelAnalysis::getLinearModelResult() const
{
  return linearModelResult_;
}

String LinearModelAnalysis::getFormula() const
{
  return linearModelResult_.getFormula();
}

Description LinearModelAnalysis::getCoefficientsNames() const
{
  return linearModelResult_.getCoefficientsNames();
}

NumericalSample LinearModelAnalysis::getResiduals() const
{
  return linearModelResult_.getSampleResiduals();
}

NumericalSample LinearModelAnalysis::getStandardizedResiduals() const
{
  return linearModelResult_.getStandardizedResiduals();
}

NumericalSample LinearModelAnalysis::getCoefficientsEstimates() const
{
  NumericalPoint beta(linearModelResult_.getTrendCoefficients());
  NumericalSample result(beta.getDimension(), 1);
  for (UnsignedInteger i = 0; i < beta.getDimension(); ++i)
  {
    result(i, 0) = beta[i];
  }
  return result;
}

NumericalSample LinearModelAnalysis::getCoefficientsStandardErrors() const
{
  const NumericalScalar sigma2(getResiduals().computeRawMoment(2)[0]);
  const NumericalPoint leverages(linearModelResult_.getLeverages());
  const UnsignedInteger n = leverages.getSize();
  NumericalSample standardErrors(n, 1);
  for (UnsignedInteger i = 0; i < standardErrors.getSize(); ++i)
  {
    standardErrors(i, 0) = std::sqrt(sigma2 * leverages[i]);
  }
  return standardErrors;

}

NumericalSample LinearModelAnalysis::getCoefficientsTScores() const
{
  const NumericalSample estimates(getCoefficientsEstimates());
  const NumericalSample standardErrors(getCoefficientsStandardErrors());
  NumericalSample tScores(estimates.getSize(), 1);
  for (UnsignedInteger i = 0; i < tScores.getSize(); ++i)
  {
    tScores(i, 0) = estimates(i, 0) / standardErrors(i, 0);
  }
  return tScores;
}

NumericalSample LinearModelAnalysis::getCoefficientsPValues() const
{
  throw NotYetImplementedException(HERE);
}

/* Leverages */
NumericalPoint LinearModelAnalysis::getLeverages() const
{
  return linearModelResult_.getLeverages();
}

/* Cook's distances */
NumericalPoint LinearModelAnalysis::getCookDistances() const
{
  return linearModelResult_.getCookDistances();
}

/* Number of degrees of freedom */
UnsignedInteger LinearModelAnalysis::getDegreesOfFreedom() const
{
  const UnsignedInteger n = linearModelResult_.getLeverages().getSize();
  const UnsignedInteger pPlusOne = linearModelResult_.getCoefficientsNames().getSize();
  return n - pPlusOne;
}

/* R-squared test */
NumericalScalar LinearModelAnalysis::getRSquared() const
{
  throw NotYetImplementedException(HERE);
}

/* Adjusted R-squared test */
NumericalScalar LinearModelAnalysis::getAdjustedRSquared() const
{
  throw NotYetImplementedException(HERE);
}

/* Fisher test */
NumericalScalar LinearModelAnalysis::getFisherScore() const
{
  throw NotYetImplementedException(HERE);
}

NumericalScalar LinearModelAnalysis::getFisherPValue() const
{
  throw NotYetImplementedException(HERE);
}

/* Kolmogorov-Smirnov normality test */
TestResult LinearModelAnalysis::getNormalityTestResultKolmogorovSmirnov() const
{
  throw NotYetImplementedException(HERE);
}

/* Anderson-Darling normality test */
TestResult LinearModelAnalysis::getNormalityTestResultAndersonDarling() const
{
  return NormalityTest::AndersonDarlingNormal(linearModelResult_.getSampleResiduals());
}

/* Chi-Squared normality test */
TestResult LinearModelAnalysis::getNormalityTestResultChiSquared() const
{
  throw NotYetImplementedException(HERE);
}

/* [1] Draw a plot of residuals versus fitted values */
Graph LinearModelAnalysis::drawResidualsVsFitted() const
{
  const NumericalSample inputData(linearModelResult_.getInputSample());
  const NumericalMathFunction metamodel(linearModelResult_.getMetaModel());
  const NumericalSample fitted(metamodel(inputData));
  const NumericalSample residuals(getResiduals());
  const UnsignedInteger size(fitted.getSize());
  NumericalSample dataFull(fitted);
  dataFull.stack(residuals);
  Graph graph("Residuals vs Fitted", "Fitted values", "Residuals", true, "topright");
  Cloud cloud(dataFull, "black", "fcircle");
  graph.add(cloud);
  // Add point identifiers for worst residuals
  UnsignedInteger identifiers(ResourceMap::GetAsUnsignedInteger("LinearModelAnalysis-Identifiers"));
  if (identifiers > 0)
  {
    if (identifiers > size)
      identifiers = size;
    Description annotations(size);
    NumericalSample dataWithIndex(size, 2);
    for(UnsignedInteger i = 0; i < size; ++i)
    {
      dataWithIndex(i, 0) = std::abs(residuals(i, 0));
      dataWithIndex(i, 1) = i;
    }
    const NumericalSample sortedData(dataWithIndex.sortAccordingToAComponent(0));
    Indices positions(size);
    for(UnsignedInteger i = 0; i < identifiers; ++i)
    {
      const UnsignedInteger index = sortedData(size - 1 - i, 1);
      annotations[index] = (OSS() << index + 1);
      if (residuals(index, 0) < 0.0)
        positions[index] = 3;
      else
        positions[index] = 1;
    }
    Text text(dataFull, annotations, 1);
    text.setColor("red");
    text.setTextPositions(positions);
    graph.add(text);
  }
  // Adapt the margins
  NumericalPoint boundingBox(graph.getBoundingBox());
  NumericalScalar width = boundingBox[1] - boundingBox[0];
  NumericalScalar height = boundingBox[3] - boundingBox[2];
  boundingBox[0] -= 0.1 * width;
  boundingBox[1] += 0.1 * width;
  boundingBox[2] -= 0.1 * height;
  boundingBox[3] += 0.1 * height;
  graph.setBoundingBox(boundingBox);
  return graph;
}

/* [2] a Scale-Location plot of sqrt(| residuals |) versus fitted values */
Graph LinearModelAnalysis::drawScaleLocation() const
{
  const NumericalSample inputData(linearModelResult_.getInputSample());
  const NumericalMathFunction metamodel(linearModelResult_.getMetaModel());
  const NumericalSample fitted(metamodel(inputData));
  const NumericalSample stdresiduals(getStandardizedResiduals());
  const UnsignedInteger size(fitted.getSize());
  NumericalSample dataFull(fitted);
  NumericalSample sqrtstdresiduals(size,1);
  for(UnsignedInteger i = 0; i < size; ++i)
  {
    sqrtstdresiduals(i, 0) = std::sqrt(std::abs(stdresiduals(i, 0)));
  }
  dataFull.stack(sqrtstdresiduals);
  Graph graph("Scale-Location", "Fitted values", "|Std. residuals|^0.5", true, "topright");
  Cloud cloud(dataFull, "black", "fcircle");
  graph.add(cloud);
  // Add point identifiers for worst standardized residuals
  UnsignedInteger identifiers(ResourceMap::GetAsUnsignedInteger("LinearModelAnalysis-Identifiers"));
  if (identifiers > 0)
  {
    if (identifiers > size)
      identifiers = size;
    Description annotations(size);
    NumericalSample dataWithIndex(size, 2);
    for(UnsignedInteger i = 0; i < size; ++i)
    {
      dataWithIndex(i, 0) = std::abs(stdresiduals(i, 0));
      dataWithIndex(i, 1) = i;
    }
    const NumericalSample sortedData(dataWithIndex.sortAccordingToAComponent(0));
    Indices positions(size);
    for(UnsignedInteger i = 0; i < identifiers; ++i)
    {
      const UnsignedInteger index = sortedData(size - 1 - i, 1);
      annotations[index] = (OSS() << index + 1);
      if (stdresiduals(index, 0) < 0.0)
        positions[index] = 3;
      else
        positions[index] = 1;
    }
    Text text(dataFull, annotations, 1);
    text.setColor("red");
    text.setTextPositions(positions);
    graph.add(text);
  }
  // Adapt the margins
  NumericalPoint boundingBox(graph.getBoundingBox());
  NumericalScalar width = boundingBox[1] - boundingBox[0];
  NumericalScalar height = boundingBox[3] - boundingBox[2];
  boundingBox[0] -= 0.1 * width;
  boundingBox[1] += 0.1 * width;
  boundingBox[2] -= 0.1 * height;
  boundingBox[3] += 0.1 * height;
  graph.setBoundingBox(boundingBox);
  return graph;
}

/* [3] a Normal quantiles-quantiles plot of standardized residuals */
Graph LinearModelAnalysis::drawQQplot() const
{
  const NumericalSample stdresiduals(getStandardizedResiduals());
  const UnsignedInteger size(stdresiduals.getSize());
  const Normal distribution(1); // Standart normal distribution 
  const NumericalSample sortedSample(stdresiduals.sort(0));
  NumericalSample dataFull(size, 2);
  const NumericalScalar step = 1.0 / size;
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    dataFull[i][1] = sortedSample[i][0];
    dataFull[i][0] = distribution.computeQuantile((i + 0.5) * step)[0];
  }
  Graph graph("Normal Q-Q", "Theoretical Quantiles", "Std. residuals", true, "topright");
  Cloud cloud(dataFull, "black", "fcircle");
  graph.add(cloud);
  // Add point identifiers for worst standardized residuals
  UnsignedInteger identifiers(ResourceMap::GetAsUnsignedInteger("LinearModelAnalysis-Identifiers"));
  if (identifiers > 0)
  {
    if (identifiers > size)
      identifiers = size;
    Description annotations(size);
    NumericalSample dataWithIndex1(size, 2);
    NumericalSample dataWithIndex2(size, 2);
    for(UnsignedInteger i = 0; i < size; ++i)
    {
      dataWithIndex1(i, 0) = std::abs(dataFull(i, 1));
      dataWithIndex1(i, 1) = i;
      dataWithIndex2(i, 0) = std::abs(stdresiduals(i, 0));
      dataWithIndex2(i, 1) = i;

    }
    const NumericalSample sortedData1(dataWithIndex1.sortAccordingToAComponent(0));
    const NumericalSample sortedData2(dataWithIndex2.sortAccordingToAComponent(0));
    Indices positions(size);
    for(UnsignedInteger i = 0; i < identifiers; ++i)
    {
      const UnsignedInteger index1 = sortedData1(size - 1 - i, 1);
      const UnsignedInteger index2 = sortedData2(size - 1 - i, 1);
      annotations[index1] = (OSS() << index2 + 1);
      if (dataFull(index1, 1) < 0.0)
        positions[index1] = 3;
      else
        positions[index1] = 1;
    }
    Text text(dataFull, annotations, 1);
    text.setColor("red");
    text.setTextPositions(positions);
    graph.add(text);
  }
  // add line to a normal QQ plot which passes through the first and third quartiles
  NumericalSample diagonal(2, 2);
  NumericalPoint point(2);
  const UnsignedInteger id1Q = 0.25*size-0.5;
  const UnsignedInteger id3Q = 0.75*size-0.5;
  point[0] = (dataFull[id3Q][1]-dataFull[id1Q][1])/(dataFull[id3Q][0]-dataFull[id1Q][0]);
  point[1] = dataFull[id3Q][1]-point[0]* dataFull[id3Q][0];  
  diagonal[0][0] = dataFull[0][0];
  diagonal[0][1] = dataFull[0][0]*point[0]+point[1];
  diagonal[1][0] = dataFull[size-1][0];
  diagonal[1][1] = dataFull[size-1][0]*point[0]+point[1];
  Curve curve(diagonal, "red", "dashed", 2);
  graph.add(curve);
  // Adapt the margins
  NumericalPoint boundingBox(graph.getBoundingBox());
  NumericalScalar width = boundingBox[1] - boundingBox[0];
  NumericalScalar height = boundingBox[3] - boundingBox[2];
  boundingBox[0] -= 0.1 * width;
  boundingBox[1] += 0.1 * width;
  boundingBox[2] -= 0.1 * height;
  boundingBox[3] += 0.1 * height;
  graph.setBoundingBox(boundingBox);
  return graph;
}

/* [4] a plot of Cook's distances versus row labels */
Graph LinearModelAnalysis::drawCookDistance() const
{
  const NumericalPoint cookdistances(getCookDistances());
  const UnsignedInteger size(cookdistances.getSize());
  // Add point identifiers for worst Cook's distance
  UnsignedInteger identifiers(ResourceMap::GetAsUnsignedInteger("LinearModelAnalysis-Identifiers"));
  Description annotations(size);
  if (identifiers > 0)
  {
    if (identifiers > size)
      identifiers = size;
    NumericalSample dataWithIndex(size, 2);
    for(UnsignedInteger i = 0; i < size; ++i)
    {
      dataWithIndex(i, 0) = cookdistances[i];
      dataWithIndex(i, 1) = i;
    }
    const NumericalSample sortedData(dataWithIndex.sortAccordingToAComponent(0));
    for(UnsignedInteger i = 0; i < identifiers; ++i)
    {
      const UnsignedInteger index = sortedData(size - 1 - i, 1);
      annotations[index] = (OSS() << index + 1);
    }
  }
  Graph graph("Cook's distance", "Obs. number", "Cook's distance", true, "topright");
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    NumericalSample dataFull(2, 2);
    dataFull(0, 0) = i+1;
    dataFull(0, 1) = 0.0;
    dataFull(1, 0) = i+1;
    dataFull(1, 1) = cookdistances[i]; 
    Curve curve(dataFull, "black", "solid", 2);
    graph.add(curve);
    if (annotations[i] != "")
    { 
      Description desc(2);
      desc[1]=annotations[i];
      Text text(dataFull, desc, 3);
      text.setColor("red");
      graph.add(text);
    }
  }
  // Adapt the margins
  NumericalPoint boundingBox(graph.getBoundingBox());
  NumericalScalar height = boundingBox[3] - boundingBox[2];
  boundingBox[2] -= 0.1 * height;
  boundingBox[3] += 0.1 * height;
  graph.setBoundingBox(boundingBox);
  return graph;
}

/* [5] a plot of residuals versus leverages that adds bands corresponding to Cook's distances of 0.5 and 1. */
Graph LinearModelAnalysis::drawResidualsVsLeverages() const
{
  const NumericalPoint cookdistances(getCookDistances());
  const NumericalPoint leverages(getLeverages());
  const NumericalSample stdresiduals(getStandardizedResiduals());
  const UnsignedInteger size(stdresiduals.getSize());
  NumericalSample leveragesS(size, 1);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
   leveragesS(i, 0) = leverages[i];
  }
  NumericalSample dataFull(leveragesS);
  dataFull.stack(stdresiduals);
  Graph graph("Residuals vs Leverage", "Leverage", "Std. residuals", true, "topright");
  Cloud cloud(dataFull, "black", "fcircle");
  graph.add(cloud);
  // Add point identifiers for worst Cook's distance
  UnsignedInteger identifiers(ResourceMap::GetAsUnsignedInteger("LinearModelAnalysis-Identifiers"));
  if (identifiers > 0)
  {
    if (identifiers > size)
      identifiers = size;
    Description annotations(size);
    NumericalSample dataWithIndex(size, 2);
    for(UnsignedInteger i = 0; i < size; ++i)
    {
      dataWithIndex(i, 0) = cookdistances[i];
      dataWithIndex(i, 1) = i;
    }
    const NumericalSample sortedData(dataWithIndex.sortAccordingToAComponent(0));
    Indices positions(size);
    for(UnsignedInteger i = 0; i < identifiers; ++i)
    {
      const UnsignedInteger index = sortedData(size - 1 - i, 1);
      annotations[index] = (OSS() << index + 1);
      if (cookdistances[index] < 0.0)
        positions[index] = 3;
      else
        positions[index] = 1;
    }
    Text text(dataFull, annotations, 1);
    text.setColor("red");
    text.setTextPositions(positions);
    graph.add(text);
  }
  // Adapt the margins
  NumericalPoint boundingBox(graph.getBoundingBox());
  NumericalScalar width = boundingBox[1] - boundingBox[0];
  NumericalScalar height = boundingBox[3] - boundingBox[2];
  boundingBox[0] -= 0.1 * width;
  boundingBox[1] += 0.1 * width;
  boundingBox[2] -= 0.1 * height;
  boundingBox[3] += 0.1 * height;
  graph.setBoundingBox(boundingBox);
  // Add contour plot of Cook's distance
  const UnsignedInteger pPlusOne = linearModelResult_.getCoefficientsNames().getSize();
  const UnsignedInteger step = 100;
  NumericalScalar ptx;  
  NumericalSample diagonal1(2,2);
  NumericalSample diagonal2(2,2);
  for(UnsignedInteger i = 0; i < step-1; ++i)
  {
    ptx = boundingBox[0] + i*(boundingBox[1] - boundingBox[0])/step;
    diagonal1[0][0] = ptx;
    diagonal1[0][1] = std::sqrt(1.0*pPlusOne*(1.0-ptx)/ptx);
    diagonal2[0][0] = ptx;
    diagonal2[0][1] = std::sqrt(0.5*pPlusOne*(1.0-ptx)/ptx);
    ptx = boundingBox[0] + (i+1)*(boundingBox[1] - boundingBox[0])/step;
    diagonal1[1][0] = ptx;
    diagonal1[1][1] = std::sqrt(1.0*pPlusOne*(1.0-ptx)/ptx);
    diagonal2[1][0] = ptx;
    diagonal2[1][1] = std::sqrt(0.5*pPlusOne*(1.0-ptx)/ptx);
    Curve curve1(diagonal1, "red", "solid", 2);
    graph.add(curve1);
    Curve curve2(diagonal2, "blue", "solid", 2);
    graph.add(curve2);
  }
  return graph;
}

/* [6] a plot of Cook's distances versus leverage/(1-leverage) */
Graph LinearModelAnalysis::drawCookVsLeverages() const
{
  const NumericalPoint leverages(getLeverages());
  const NumericalPoint cookdistances(getCookDistances());
  const UnsignedInteger size(cookdistances.getSize());
  NumericalSample dataFull(size, 2);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
   dataFull(i, 0) = leverages[i] / (1.0 - leverages[i]);
   dataFull(i, 1) = cookdistances[i];
  }
  Graph graph("Cook's dist vs Leverage h[ii]/(1-h[ii])", "Leverage h[ii]/(1-h[ii])", "Cook's distance", true, "topright");
  Cloud cloud(dataFull, "black", "fcircle");
  graph.add(cloud);
  // Add point identifiers for worst Cook's distance
  UnsignedInteger identifiers(ResourceMap::GetAsUnsignedInteger("LinearModelAnalysis-Identifiers"));
  if (identifiers > 0)
  {
    if (identifiers > size)
      identifiers = size;
    Description annotations(size);
    NumericalSample dataWithIndex(size, 2);
    for(UnsignedInteger i = 0; i < size; ++i)
    {
      dataWithIndex(i, 0) = std::abs(cookdistances[i]);
      dataWithIndex(i, 1) = i;
    }
    const NumericalSample sortedData(dataWithIndex.sortAccordingToAComponent(0));
    Indices positions(size);
    for(UnsignedInteger i = 0; i < identifiers; ++i)
    {
      const UnsignedInteger index = sortedData(size - 1 - i, 1);
      annotations[index] = (OSS() << index + 1);
      if (cookdistances[index] < 0.0)
        positions[index] = 3;
      else
        positions[index] = 1;
    }
    Text text(dataFull, annotations, 1);
    text.setColor("red");
    text.setTextPositions(positions);
    graph.add(text);
  }
  // Adapt the margins
  NumericalPoint boundingBox(graph.getBoundingBox());
  NumericalScalar width = boundingBox[1] - boundingBox[0];
  NumericalScalar height = boundingBox[3] - boundingBox[2];
  boundingBox[0] -= 0.1 * width;
  boundingBox[1] += 0.1 * width;
  boundingBox[2] -= 0.1 * height;
  boundingBox[3] += 0.1 * height;
  graph.setBoundingBox(boundingBox);
  return graph;
}

/* Method save() stores the object through the StorageManager */
void LinearModelAnalysis::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "linearModelResult_", linearModelResult_ );
}


/* Method load() reloads the object from the StorageManager */
void LinearModelAnalysis::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "linearModelResult_", linearModelResult_ );
}

void LinearModelAnalysis::InitializeResourceMap()
{
  UnsignedInteger identifiers = 0;
  try
  {
    identifiers = ResourceMap::GetAsUnsignedInteger("LinearModelAnalysis-Identifiers");
  }
  catch(InternalException)
  {
    ResourceMap::SetAsUnsignedInteger("LinearModelAnalysis-Identifiers", 3);
  }
}

END_NAMESPACE_OPENTURNS
