//                                               -*- C++ -*-
/**
 *  @brief Text class for plot labels
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
#include "openturns/Text.hxx"
#include "openturns/PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Text);

static const Factory<Text> Factory_Text;

/* Default constructor */
Text::Text(const NumericalSample & data,
           const String & legend)
  : DrawableImplementation(data, legend)
{
  // Check data validity
  setData(data);
}

/* Constructor from complex numbers */
Text::Text(const NumericalComplexCollection & data,
           const String & legend)
  : DrawableImplementation(NumericalSample(0, 2), legend)
{
  // Convert the complex numbers into a NumericalSample
  const UnsignedInteger size = data.getSize();
  NumericalSample sample(size, 2);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    sample[i][0] = data[i].real();
    sample[i][1] = data[i].imag();
  }
  // Check data validity
  setData(sample);
}

/* Contructor from 2 data sets */
Text::Text(const NumericalSample & dataX,
           const NumericalSample & dataY,
           const String & legend)
  : DrawableImplementation(NumericalSample(0, 2), legend)
{
  const UnsignedInteger size = dataX.getSize();
  if (dataY.getSize() != size) throw InvalidArgumentException(HERE) << "Error: cannot build a Text based on two numerical samples with different size.";
  if ((dataX.getDimension() != 1) || (dataY.getDimension() != 1)) throw InvalidArgumentException(HERE) << "Error: cannot build a Text based on two numerical samples of dimension greater than 1.";
  NumericalSample dataFull(dataX);
  dataFull.stack(dataY);
  // Check data validity
  setData(dataFull);
}

Text::Text(const NumericalPoint & dataX,
           const NumericalPoint & dataY,
           const String & legend)
  : DrawableImplementation(NumericalSample(0, 2), legend)
{
  const UnsignedInteger size = dataX.getDimension();
  if (dataY.getDimension() != size) throw InvalidArgumentException(HERE) << "Error: cannot build a Text based on two numerical points with different dimension.";
  NumericalSample dataFull(size, 2);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    dataFull[i][0] = dataX[i];
    dataFull[i][1] = dataY[i];
  }
  // Check data validity
  setData(dataFull);
}

/* Constructor with parameters */
Text::Text(const NumericalSample & data,
           const String & color,
           const String & pointStyle,
           const String & legend)
  : DrawableImplementation(data, legend)
{
  // Check data validity
  setData(data);
  setColor(color);
  setPointStyle(pointStyle);
}

/* String converter */
String Text::__repr__() const
{
  OSS oss;
  oss << "class=" << Text::GetClassName()
      << " name=" << getName()
      << " derived from " << DrawableImplementation::__repr__();
  return oss;
}

/* Draw method */
String Text::draw() const
{
  dataFileName_ = "";
  OSS oss;
  if (text_.getSize() != 0)
  {
    oss << "labels <- rep(\"\", " << text_.getSize() << ")\n";
    // We assume that only few labels are printed, otherwise graph is ugly
    for (UnsignedInteger i = 0; i < text_.getSize(); ++i)
    {
      if (text_[i] != "")
      {
        oss << "labels[" << (i+1) << "] <- \"" << text_[i] << "\"\n";
      }
    }
    oss << "pos <- c(4,2)\n"
        << "indices <- which(labels != \"\")\n";
    // Adjust label horizontally
    oss << "labelpos <- pos[1+as.numeric(dataOT[indices,1] > mean(range(dataOT[,1])))]\n";
    // oss << "labelpos <- 3\n";
    oss << "text(dataOT[indices,1], dataOT[indices,2], labels[indices], cex = 0.75, xpd = TRUE, pos = labelpos, offset = 0.25)\n";
  }
  }
  return oss;
}

/* Clone method */
Text * Text::clone() const
{
  return new Text(*this);
}

/* Check validity of data */
void Text::checkData(const NumericalSample & data) const
{
  if (data.getDimension() != 2)
  {
    throw InvalidDimensionException(HERE) << "Expected sample of dimension 2: got " << data.getDimension();
  }
}

/* Method save() stores the object through the StorageManager */
void Text::save(Advocate & adv) const
{
  DrawableImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void Text::load(Advocate & adv)
{
  DrawableImplementation::load(adv);
}



END_NAMESPACE_OPENTURNS
