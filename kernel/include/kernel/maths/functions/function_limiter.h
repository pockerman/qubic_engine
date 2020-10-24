#ifndef FUNCTION_LIMITER_H
#define FUNCTION_LIMITER_H

#include "kernel/base/types.h"
#include "kernel/maths/functions/function_base.h"
#include <vector>

namespace kernel{
namespace maths{
namespace functions{

///
/// \brief The class FunctionLimiter
///
template<typename FunctionTp>
class FunctionLimiter: public FunctionBase<typename FunctionTp::output_t,
                                            typename FunctionTp::input_t>
{
public:

    typedef FunctionTp function_t;
    typedef typename function_t::input_t input_t;
    typedef typename function_t::output_t output_t;

    ///
    /// \brief Constructor
    ///
    FunctionLimiter(function_t& function);

    ///
    /// \brief Returns the value of the function
    ///
    virtual output_t value(const input_t& input)const;

    ///
    /// \brief Returns the number of coefficients
    ///
    virtual uint_t n_coeffs()const final{return function_ptr_->n_coeffs(); }

    ///
    /// \brief Returns the gradient of the function for the i-th coefficient
    ///
    virtual real_t coeff_grad(uint_t i, const DynVec<real_t>& point)const
    {return function_ptr_->coeff_grad(i, point);}

    ///
    /// \brief Returns the gradient of the function for the i-th coefficient
    ///
    DynVec<real_t> coeff_grads(const DynVec<real_t>& point)const
    {return function_ptr_->coeff_grads(point);}

    ///
    /// \brief Returns the gradient of the function for the i-th variable
    ///
    virtual real_t grad(uint_t i, const DynVec<real_t>& point)const
    {return function_ptr_->grad(i, point);}

    ///
    /// \brief Returns the gradients of the function
    ///
    virtual DynVec<real_t> gradients(const DynVec<real_t>& point)const
    {return function_ptr_->gradients(point);}

    ///
    /// \brief Returns the coefficients of the underlying monomials
    ///
    std::vector<real_t> coeffs()const
    {return function_ptr_->coeffs();}

    ///
    /// \brief Returns the coefficients of the underlying monomials
    ///
    void set_coeffs(const std::vector<real_t>& coeffs)
    {function_ptr_->set_coeffs(coeffs);}

    ///
    /// \brief Returns the i-th coefficient
    ///
    real_t coeff(uint_t c)const
    {return function_ptr_->coeff(c);}

    bool is_max_clipped()const{return use_max_clip_value_;}
    bool is_min_clipped()const{return use_min_clip_value_;}
    void set_use_max_clip_value_flag(bool flag){use_max_clip_value_=flag;}
    void set_use_min_clip_value_flag(bool flag){use_min_clip_value_=flag;}
    void set_max_clip_value(real_t val){max_clip_value_=val;}
    void set_min_clip_value(real_t val){min_clip_value_=val;}
    real_t get_max_clip_value()const{return max_clip_value_;}
    real_t get_min_clip_value()const{return min_clip_value_;}
    real_t get_max_clip_limit_value()const{return max_clip_limit_;}
    real_t get_min_clip_limit_value()const{return min_clip_limit_;}
    void set_max_limit_clip_value(real_t val){max_clip_limit_=val;}
    void set_min_limit_clip_value(real_t val){min_clip_limit_=val;}



private:

    ///
    /// \brief function_ptr_. The function to limit
    ///
    function_t* function_ptr_;

    ///
    /// \brief use_max_clip_value_ Flag indicating that the
    /// value of the function should be clipped
    ///
    bool use_max_clip_value_{false};

    ///
    /// \brief max_clip_limit_. The value above which
    /// the max_clip_value_ is set
    ///
    real_t max_clip_limit_{0.0};

    ///
    /// \brief max_clip_value_. The max clip value
    ///
    real_t max_clip_value_{0.0};

    ///
    /// \brief use_min_clip_value_ Flag indicating that the
    /// value of the function should be clipped
    ///
    bool use_min_clip_value_{false};

    ///
    /// \brief min_clip_limit_. The value below which
    /// the min_clip_value_ is set
    ///
    real_t min_clip_limit_{0.0};

    ///
    /// \brief min_clip_value_. The max clip value
    ///
    real_t min_clip_value_{0.0};

};

template<typename FunctionTp>
FunctionLimiter<FunctionTp>::FunctionLimiter(FunctionTp& function)
    :
      FunctionBase<typename FunctionTp::output_t,
                   typename FunctionTp::input_t>(),
      function_ptr_(&function)
{}

template<typename FunctionTp>
typename FunctionLimiter<FunctionTp>::output_t
FunctionLimiter<FunctionTp>::value(const input_t& input)const{

    auto result = function_ptr_->value(input);

    if(is_max_clipped() && result > get_max_clip_limit_value() ){

        result = get_max_clip_value();
    }
    else if(is_min_clipped() && result < get_min_clip_limit_value()){
        result = get_min_clip_value();
    }

    return result;
}

}
}

}

#endif // FUNCTION_LIMITER_H
