//                                               -*- C++ -*-
/**
 *  @brief The test file of class Triangular for standard methods
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
#include "openturns/OT.hxx"
#include "openturns/OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);
  setRandomGenerator();

  try
  {
    Triangular distribution(1., 2.5, 4.0);
    UnsignedInteger size = 10000;
    NumericalSample sample(distribution.getSample(size));
    TriangularFactory factory;
    CovarianceMatrix covariance;
    // Distribution estimatedDistribution(factory.build(sample, covariance));
    Distribution estimatedDistribution(factory.build(sample));
    fullprint << "Distribution          =" << distribution << std::endl;
    fullprint << "Estimated distribution=" << estimatedDistribution << std::endl;
    // fullprint << "Covariance=" << covariance << std::endl;
    estimatedDistribution = factory.build();
    fullprint << "Default distribution=" << estimatedDistribution << std::endl;
    estimatedDistribution = factory.build(distribution.getParameter());
    fullprint << "Distribution from parameters=" << estimatedDistribution << std::endl;
    Triangular estimatedTriangular(factory.buildAsTriangular(sample));
    fullprint << "Triangular          =" << distribution << std::endl;
    fullprint << "Estimated triangular=" << estimatedTriangular << std::endl;
    // fullprint << "Covariance=" << covariance << std::endl;
    estimatedTriangular = factory.buildAsTriangular();
    fullprint << "Default triangular=" << estimatedTriangular << std::endl;
    estimatedTriangular = factory.buildAsTriangular(distribution.getParameter());
    fullprint << "Triangular from parameters=" << estimatedTriangular << std::endl;
    // Test for constant sample
    sample = NumericalSample(size, NumericalPoint(1, 0.0));
    estimatedDistribution = factory.build(sample);
    fullprint << "Estimated distribution=" << estimatedDistribution << std::endl;
    sample = NumericalSample(size, NumericalPoint(1, 1.0));
    estimatedDistribution = factory.build(sample);
    fullprint << "Estimated distribution=" << estimatedDistribution << std::endl;
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }


  return ExitCode::Success;
}
