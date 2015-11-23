// SWIG file model_copula_module.i

%module(package="openturns", docstring="Copulas") model_copula
#pragma SWIG nowarn=302,509
//%feature("autodoc","1");

%{
#include "OTconfig.hxx"
#include "OTBase.hxx"
#include "OTModel.hxx"
#include "OTDistribution.hxx"
%}

%include typemaps.i
%include OTtypes.i
%include OTexceptions.i
%include std_vector.i

%import base_module.i

/* Uncertainty/Model */
%include DistributionImplementation.i
%include Distribution.i
%include DistributionFactoryResult.i
%include DistributionParametersImplementation.i
%include DistributionParameters.i
%include DistributionImplementationFactory.i
%include DistributionFactory.i
%include ContinuousDistribution.i
%include EllipticalDistribution.i
%include DiscreteDistribution.i
%include CopulaImplementation.i
%include Copula.i
%include ArchimedeanCopula.i
%include SklarCopula.i

/* Uncertainty/Distribution (copulas) */
%include AliMikhailHaqCopula.i
%include AliMikhailHaqCopulaFactory.i
%include IndependentCopula.i
%include MinCopula.i
%include NormalCopula.i
%include NormalCopulaFactory.i
%include FarlieGumbelMorgensternCopula.i
%include FarlieGumbelMorgensternCopulaFactory.i
%include FrankCopula.i
%include FrankCopulaFactory.i
%include ClaytonCopula.i
%include ClaytonCopulaFactory.i
%include GumbelCopula.i
%include GumbelCopulaFactory.i
%include ComposedDistribution.i
%include ComposedCopula.i
%include OrdinalSumCopula.i

/* At last we include template definitions */
%include UncertaintyModelCopulaTemplateDefs.i
