#ifndef FUNCTION_BASE_H
#define FUNCTION_BASE_H


#include "kernel/base/types.h"

namespace kernel
{

template<typename OutputTp, typename... InputTp>
class FunctionBase
{


public:

    typedef OutputTp output_t;

    /// \brief Destructor
    virtual ~FunctionBase()
    {}

    /// \brief Returns the value of the function
    virtual output_t value(const InputTp&...  input)const=0;

    /// \brief Returns the gradients of the function
    virtual DynVec<real_t> gradients(const InputTp&...  input)const=0;

    /// \brief Returns the number of coefficients
    virtual uint_t n_coeffs()const=0;

protected:

    /// \brief Constructor
    FunctionBase();
};


template<typename OutputTp, typename... InputTp>
inline
FunctionBase<OutputTp, InputTp...>::FunctionBase()
{}

}

#endif // FUNCTION_BASE_H
