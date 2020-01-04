#ifndef SIGMOID_FUNCTION_H
#define SIGMOID_FUNCTION_H

#include "kernel/maths/functions/function_base.h"
#include <cmath>

namespace kernel {

template<typename FunctionType>
class SigmoidFunction: public FunctionBase<typename FunctionType::output_t, typename FunctionType::input_t>
{

public:

    typedef  FunctionType function_t;
    typedef typename function_t::input_t input_t;
    typedef typename function_t::output_t output_t;

    /// \brief Constructor
    SigmoidFunction(const function_t& function);

    /// \brief Returns the value of the function
    virtual output_t value(const input_t&  input)const final override;

    /// \breif Returns the raw value of the function it wraps
    output_t raw_value(const input_t&  input)const{return function_ptr_->value(input);}

    /// \brief Returns the gradients of the function
    virtual DynVec<real_t> gradients(const input_t&  input)const final override;

    /// \brief Returns the coefficient gradients of the function it wraps
    DynVec<real_t> coeff_grads(const input_t&  input)const{return function_ptr_->coeff_grads(input);}

    /// \brief Returns the number of coefficients
    virtual uint_t n_coeffs()const final override{return 0;}

private:

    const function_t* function_ptr_;

};

template<typename FunctionType>
SigmoidFunction<FunctionType>::SigmoidFunction(const function_t& function)
    :
   function_ptr_(&function)
{}

template<typename FunctionType>
typename SigmoidFunction<FunctionType>::output_t
SigmoidFunction<FunctionType>::value(const input_t&  input)const{

    auto result = function_ptr_->value(input);
    return 1.0/(1.0 + std::exp(-result));
}

template<typename FunctionType>
DynVec<real_t>
SigmoidFunction<FunctionType>::gradients(const input_t&  input)const{

    return DynVec<real_t>(1, value(input)*(1.0 - value(input)));
}

}

#endif // SIGMOID_FUNCTION_H
