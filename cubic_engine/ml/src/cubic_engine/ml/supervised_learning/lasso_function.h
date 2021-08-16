#ifndef LASSO_FUNCTION_H
#define LASSO_FUNCTION_H

#include "cubic_engine/base/config.h"

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/maths/functions/function_base.h"
#include <cmath>

namespace cengine
{

/// Lasso regularization for the function given as a template parameter
/// The workings of the Lasso regularization can be read here
///
template<typename FunctionTp, typename...InputTp>
class LassoFunction: public kernel::FunctionBase<real_t, InputTp...>
{

public:

    typedef typename kernel::FunctionBase<real_t, InputTp...>::output_t output_t;

    /// \brief Constructor
    LassoFunction(const FunctionTp& function, real_t lambda,
                  uint_t coeff_start, uint_t coeff_end);

    /// \brief Destructor
    ~LassoFunction(){}

    /// \brief Returns the value of the regularizer
    virtual output_t value(const InputTp&...  input)const override final;

    /// \brief Returns the gradients of the function
    virtual DynVec<real_t> gradients(const InputTp&...)const override final;

    /// \brief Returns the number of coefficients
    virtual uint_t n_coeffs()const override final {return 1;}

    /// \brief Set the lambda coefficient
    void set_lambda_coeff(real_t l){lambda_ = l;}

    /// \brief Returns the lambda coefficient
    real_t get_lambda_coeff()const{return lambda_;}

    /// \brief Set the start coeff
    void set_start_coeff(uint_t c){coeff_start_ = c;}

    /// \brief Set ending coeff
    void set_end_coeff(uint_t c){coeff_end_ = c;}

    /// \brief Set start/end coeffs
    void set_start_end_coeffs(uint_t s, uint_t e);

private:

    /// \brief The underlying function that regularization
    /// is used
    const FunctionTp* function_;

    /// \brief The lambda coefficient
    real_t lambda_;

    /// \brief Starting point of the coefficients to include
    uint_t coeff_start_;

    /// \brief ending (not included) point of the coefficients to include
    uint_t coeff_end_;

};

template<typename FunctionTp, typename...InputTp>
LassoFunction<FunctionTp, InputTp...>::LassoFunction(const FunctionTp& function, real_t lambda,
                                                     uint_t coeff_start, uint_t coeff_end)
    :
    kernel::FunctionBase<real_t, InputTp...>(),
    function_(&function),
    lambda_(lambda),
    coeff_start_(coeff_start),
    coeff_end_(coeff_end)
{}

template<typename FunctionTp, typename... InputTp>
typename LassoFunction<FunctionTp, InputTp...>::output_t
LassoFunction<FunctionTp, InputTp...>::value(const InputTp&...  /*input*/)const{

    auto coeffs = function_->coeffs();

    real_t sum = 0.0;

    for(int c=coeff_start_; c<coeff_end_; ++c){

        real_t coeff = coeffs[c];
        sum += std::fabs(coeff);
    }

    return lambda_*sum;
}

template<typename FunctionTp, typename...InputTp>
DynVec<real_t>
LassoFunction<FunctionTp, InputTp...>::gradients(const InputTp&...)const{

#ifdef USE_LOG
    kernel::Logger::log_warning("LassoFunction<FunctionTp>::gradients should not be called");
#endif

    return DynVec<real_t>();
}
}

#endif // LASSO_FUNCTION_H
