//                                               -*- C++ -*-
/**
 *  @brief An implementation class for kriging random vectors
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
#ifndef OPENTURNS_KRIGINGRANDOMVECTOR_HXX
#define OPENTURNS_KRIGINGRANDOMVECTOR_HXX

#include "openturns/UsualRandomVector.hxx"
#include "openturns/KrigingResult.hxx"

BEGIN_NAMESPACE_OPENTURNS


/**
 * @class KrigingRandomVector
 *
 * An implementation class for kriging random vectors
 */
class OT_API KrigingRandomVector
  : public UsualRandomVector
{
  CLASSNAME;

public:

  /** Default constructors */
  KrigingRandomVector(const KrigingResult & krigingResult,
                      const NumericalPoint & point);

  KrigingRandomVector(const KrigingResult & krigingResult,
                      const NumericalSample & sample);

  /** Virtual constructor */
  virtual KrigingRandomVector * clone() const;

  /** String converter */
  String __repr__() const;

  /** Get a realization of the process */
  using UsualRandomVector::getRealization;
  NumericalPoint getRealization() const;

  /** Get realizations of the process */
  using UsualRandomVector::getSample;
  NumericalSample getSample(const UnsignedInteger size) const;

  /** Kriging result accessor */
  KrigingResult getKrigingResult() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  friend class Factory<KrigingRandomVector>;

  /** Default constructor */
  KrigingRandomVector() : UsualRandomVector() {};

private:
  /** The kriging result tht allows to build the random vector */
  KrigingResult krigingResult_;

  /** The points on which we seek the realizations */
  NumericalSample sample_;

}; /* class KrigingRandomVector */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_KRIGINGRANDOMVECTOR_HXX */
