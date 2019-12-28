#ifndef REAL_VECTOR_FUNCTION_BASE_H
#define REAL_VECTOR_FUNCTION_BASE_H

#include "kernel/maths/functions/function_base.h"
#include "kernel/base/types.h"

namespace kernel
{

/**
  *Base class for modelling real vector valued functions
  */

class RealVectorValuedFunctionBase: public FunctionBase<real_t, DynVec<real_t> >
{

public:

    typedef DynVec<real_t> input_t;
    typedef FunctionBase<real_t, DynVec<real_t>>::output_t output_t;


    /**
      * Destructor
      */
    virtual ~RealVectorValuedFunctionBase(){}


    /**
      * Returns the gradient of the function for the i-th variable
      */
    virtual real_t grad(uint_t i, const DynVec<real_t>& point)const=0;


    /**
      * Returns the gradient of the function for the i-th coefficient
      */
    virtual real_t coeff_grad(uint_t i, const DynVec<real_t>& point)const=0;


protected:

    /**
      * Constructor
      */
    RealVectorValuedFunctionBase();

};

inline
RealVectorValuedFunctionBase::RealVectorValuedFunctionBase()
    :
    FunctionBase<real_t, DynVec<real_t> >()
{}

}

#endif // REAL_VECTOR_FUNCTION_BASE_H
