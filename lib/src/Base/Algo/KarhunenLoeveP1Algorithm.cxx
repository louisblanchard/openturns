//                                               -*- C++ -*-
/**
 *  @brief This class implements the computation of the Karhunen-Loeve
 *         basis and eigenvalues of a given covariance model based on
 *         P1 Lagrange approximation.
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
#include "openturns/KarhunenLoeveP1Algorithm.hxx"
#include "openturns/SquareComplexMatrix.hxx"
#include "openturns/P1LagrangeEvaluationImplementation.hxx"
#include "openturns/PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class KarhunenLoeveP1Algorithm
 */

CLASSNAMEINIT(KarhunenLoeveP1Algorithm);

static const Factory<KarhunenLoeveP1Algorithm> Algorithm_KarhunenLoeveP1Algorithm;

/* Constructor without parameters */
KarhunenLoeveP1Algorithm::KarhunenLoeveP1Algorithm()
  : KarhunenLoeveAlgorithmImplementation()
  , mesh_()
{
  // Nothing to do
}

/* Constructor with parameters */
KarhunenLoeveP1Algorithm::KarhunenLoeveP1Algorithm(const Mesh & mesh,
						   const CovarianceModel & covariance,
    const NumericalScalar threshold)
  : KarhunenLoeveAlgorithmImplementation(covariance, threshold)
  , mesh_(mesh)
{
  // Nothing to do
}

/* Virtual constructor */
KarhunenLoeveP1Algorithm * KarhunenLoeveP1Algorithm::clone() const
{
  return new KarhunenLoeveP1Algorithm( *this );
}

/* Here we discretize the following Fredholm problem:
   \int_{\Omega}C(s,t)\phi_n(s)ds=\lambda_n\phi_n(t)
   using a P1 approximation of C and \phi_n:
   C(s,t)=\sum_{i,j}C(s_i,s_j)\theta_i(s)\theta_j(t)
   \phi_n(t)=\sum_k\alpha_k^n\theta_k(t)
   where s_i,s_j are vertices of the mesh, C(s_i,s_j)\in\cS^+_d(\R), \alpha_k^n\in\R^d
   leading to:
   \forall t\in\Omega, \sum_{i,j}C(s_i,s_j)\theta_j(t)\int_{\Omega}\theta_i(s)\sum_k\alpha_k^n\theta_k(s)=\lambda_n\sum_l\alpha_k^l\theta_l(t)
   For each value of n we get d equations in \alpha^n. We write these equations for t=s_1,...,s_N the N vertices of the mesh:
   \sum_{i,j}C(s_i,s_j)\theta_j(s_m)\int_{\Omega}\theta_i(s)\sum_k\alpha_k^n\theta_k(s)=\lambda_n\sum_l\alpha_l^n\theta_l(s_m)
   ie:
   \sum_i C(s_i,s_m)\int_{\Omega}\theta_i(s)\sum_k\alpha_k^n\theta_k(s)=\lambda_n\alpha_m^n\theta_m(s_m)
   In a block-matrix form we get:
   [C(s_1,s_1) ... C(s_1,s_N)][K_11 ... K_1N][\alpha_1]             [\alpha_1]
   [    ...            ...   ][ ...      ...][   ...  ] = \lambda_n [   ...  ]
   [C(s_N,s_1) ... C(s_N,s_N)][K_N1 ... K_NN][\alpha_N]             [\alpha_N]
   Where:
   K_ij = \int_{\Omega}\theta_i(s)\theta_j(s)ds I
   with I the dxd identity matrix
*/
void KarhunenLoeveP1Algorithm::run()
{
  // Compute the gram of the mesh
  CovarianceMatrix gram(mesh_.computeP1Gram());
  const NumericalScalar epsilon = ResourceMap::GetAsNumericalScalar("KarhunenLoeveP1Algorithm-RegularizationFactor");
  if (epsilon > 0.0)
    for (UnsignedInteger i = 0; i < gram.getDimension(); ++i) gram(i, i) += epsilon;
  const UnsignedInteger numVertices = mesh_.getVerticesNumber();
  // Extend the Gram matrix of the mesh
  const UnsignedInteger dimension = covariance_.getDimension();
  const UnsignedInteger augmentedDimension = dimension * numVertices;
  CovarianceMatrix G(augmentedDimension);
  for (UnsignedInteger i = 0; i < numVertices; ++i)
  {
    for (UnsignedInteger j = 0; j <= i; ++j)
    {
      const NumericalScalar gij = gram(i, j);
      for (UnsignedInteger k = 0; k < dimension; ++k)
        G(i * dimension + k, j * dimension + k) = gij;
    } // Loop over j
  } // Loop over i
  // Discretize the covariance model
  CovarianceMatrix C(covariance_.discretize(mesh_));
  SquareMatrix M((C * G).getImplementation());
  SquareComplexMatrix eigenVectorsComplex;
  SquareMatrix::NumericalComplexCollection eigenValuesComplex(M.computeEV(eigenVectorsComplex, false));
  NumericalSample eigenPairs(augmentedDimension, augmentedDimension + 1);
  for (UnsignedInteger i = 0; i < augmentedDimension; ++i)
  {
    for (UnsignedInteger j = 0; j < augmentedDimension; ++j) eigenPairs[i][j] = eigenVectorsComplex(j, i).real();
    eigenPairs[i][augmentedDimension] = -eigenValuesComplex[i].real();
  }
  eigenPairs = eigenPairs.sortAccordingToAComponent(augmentedDimension);
  SquareMatrix eigenVectors(augmentedDimension);
  NumericalPoint eigenValues(augmentedDimension);
  for (UnsignedInteger i = 0; i < augmentedDimension; ++i)
  {
    for (UnsignedInteger j = 0; j < augmentedDimension; ++j) eigenVectors(i, j) = eigenPairs[j][i];
    eigenValues[i] = -eigenPairs[i][augmentedDimension];
  }
  LOGINFO(OSS(false) << "eigenVectors=\n" << eigenVectors << ", eigenValues=" << eigenValues);
  UnsignedInteger K = 0;
  const NumericalScalar lowerBound = threshold_ * std::abs(eigenValues[0]);
  // Find the cut-off in the eigenvalues
  while ((K < augmentedDimension) && (eigenValues[K] >= lowerBound)) ++K;
  // Reduce and rescale the eigenvectors
  MatrixImplementation transposedProjection(augmentedDimension, K);
  NumericalPoint selectedEV(K);
  Basis modes(0);
  ProcessSample modesAsProcessSample(mesh_, 0, dimension);
  NumericalSampleImplementation values(numVertices, dimension);
  UnsignedInteger index = 0;
  for (UnsignedInteger k = 0; k < K; ++k)
  {
    selectedEV[k] = eigenValues[k];
    const MatrixImplementation a(*eigenVectors.getColumn(k).getImplementation());
    const MatrixImplementation Ga(G.getImplementation()->genProd(a));
    const NumericalScalar norm = std::sqrt(a.genProd(Ga, true, false)[0]);
    const NumericalScalar factor = a[0] < 0.0 ? -1.0 / norm : 1.0 / norm;
    // Store the eigen modes in two forms
    values.setData(a * factor);
    modesAsProcessSample.add(values);
    modes.add(P1LagrangeEvaluationImplementation(modesAsProcessSample.getField(k)));
    // Build the relevant column of the transposed projection matrix
    const MatrixImplementation b(Ga * (factor / sqrt(selectedEV[k])));
    std::copy(b.begin(), b.end(), transposedProjection.begin() + index);
    index += augmentedDimension;
  }
  result_ = KarhunenLoeveResultImplementation(covariance_, threshold_, selectedEV, modes, modesAsProcessSample, transposedProjection.transpose());
}

/* Mesh accessor */
Mesh KarhunenLoeveP1Algorithm::getMesh() const
{
  return mesh_;
}

/* String converter */
String KarhunenLoeveP1Algorithm::__repr__() const
{
  OSS oss(true);
  oss << "class=" << KarhunenLoeveP1Algorithm::GetClassName()
      << ", mesh=" << mesh_;
  return oss;
}

/* String converter */
String KarhunenLoeveP1Algorithm::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class=" << KarhunenLoeveP1Algorithm::GetClassName()
      << ", mesh=" << mesh_;
  return oss;
}

/* Method save() stores the object through the StorageManager */
void KarhunenLoeveP1Algorithm::save(Advocate & adv) const
{
  KarhunenLoeveAlgorithmImplementation::save(adv);
  adv.saveAttribute( "mesh_", mesh_ );
}

/* Method load() reloads the object from the StorageManager */
void KarhunenLoeveP1Algorithm::load(Advocate & adv)
{
  KarhunenLoeveAlgorithmImplementation::load(adv);
  adv.loadAttribute( "mesh_", mesh_ );
}

END_NAMESPACE_OPENTURNS
