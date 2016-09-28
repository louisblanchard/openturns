// SWIG file LinearModelStepwiseFactory.i

%{
#include "openturns/LinearModelStepwiseFactory.hxx"
%}

%include LinearModelStepwiseFactory_doc.i

%include openturns/LinearModelStepwiseFactory.hxx
namespace OT{ %extend LinearModelStepwiseFactory { LinearModelStepwiseFactory(const LinearModelStepwiseFactory & other) { return new OT::LinearModelStepwiseFactory(other); } } }
