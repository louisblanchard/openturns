// SWIG file MonomialFactory.i

%{
#include "openturns/MonomialFactory.hxx"
%}

%include MonomialFactory_doc.i

%include openturns/MonomialFactory.hxx
namespace OT { %extend MonomialFactory { MonomialFactory(const MonomialFactory & other) { return new OT::MonomialFactory(other); } } }
