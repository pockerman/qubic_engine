#ifndef REAL_VECTOR_FUNCTION_BASE_H
#define REAL_VECTOR_FUNCTION_BASE_H

#include "kernel/maths/functions/function_base.h"
#include "kernel/base/types.h"

namespace kernel
{

///
/// \brief The RealVectorValuedFunctionBase class.
/// Base class for modelling real vector valued functions
///
class RealVectorValuedFunctionBase: public FunctionBase<real_t, DynVec<real_t> >
{

public:

    typedef DynVec<real_t> input_t;
    typedef FunctionBase<real_t, DynVec<real_t>>::output_t output_t;


    ///
    /// \brief Destructor
    ///
    virtual ~RealVectorValuedFunctionBase(){}

    ///
    /// \brief Returns the gradient of the function for the i-th variable
    ///
    virtual real_t grad(uint_t i, const DynVec<real_t>& point)const=0;


    ///
    /// \brief Returns the gradient of the function for the i-th coefficient
    ///
    virtual real_t coeff_grad(uint_t i, const DynVec<real_t>& point)const=0;


protected:

    ///
    /// \brief Constructor
    ///
    RealVectorValuedFunctionBase();

private:

};

inline
RealVectorValuedFunctionBase::RealVectorValuedFunctionBase()
    :
    FunctionBase<real_t, DynVec<real_t> >()
{}

}

#endif // REAL_VECTOR_FUNCTION_BASE_H
