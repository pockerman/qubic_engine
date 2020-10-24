#ifndef ELASTIC_NET_FUNCTION_H
#define ELASTIC_NET_FUNCTION_H

#include "cubic_engine/base/config.h"

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/maths/functions/function_base.h"
#include <cmath>

namespace cengine
{

/// ElasticNet regularization for the function given as a template parameter
/// The workings of the ElasticNet regularization can be read here
/// https://en.wikipedia.org/wiki/Elastic_net_regularization
template<typename FunctionTp, typename...InputTp>
class ElasticNetFunction: public kernel::FunctionBase<real_t, InputTp...>
{

public:

    typedef typename kernel::FunctionBase<real_t, InputTp...>::output_t output_t;

    /// \brief Constructor. lambda1 is the Lasso coeffcient
    /// and lambda2 is the Ridge coefficient
    ElasticNetFunction(const FunctionTp& function,
                       real_t lambda1, real_t lambda2,
                       uint_t coeff_start, uint_t coeff_end);

    /// \brief Destructor
    ~ElasticNetFunction(){}

    /// \brief Returns the value of the regularizer
    virtual output_t value(const InputTp&...  input)const override final;

    /// \brief Returns the gradients of the function
    virtual DynVec<real_t> gradients(const InputTp&...)const override final;

    /// \brief Returns the number of coefficients
    virtual uint_t n_coeffs()const override final {return 1;}

    /// \brief Set the lambda coefficient
    void set_lambda1_coeff(real_t l){lambda1_ = l;}

    /// \brief Returns the lambda coefficient
    real_t get_lambda1_coeff()const{return lambda1_;}

    /// \brief Set the lambda coefficient
    void set_lambda2_coeff(real_t l){lambda2_ = l;}

    /// \brief Returns the lambda coefficient
    real_t get_lambda2_coeff()const{return lambda2_;}

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
    real_t lambda1_;

    /// the second coefficient
    real_t lambda2_;

    /// \brief Starting point of the coefficients to include
    uint_t coeff_start_;

    /// \brief ending (not included) point of the coefficients to include
    uint_t coeff_end_;

};

template<typename FunctionTp, typename...InputTp>
ElasticNetFunction<FunctionTp, InputTp...>::ElasticNetFunction(const FunctionTp& function,
                                                               real_t lambda1, real_t lambda2,
                                                                uint_t coeff_start, uint_t coeff_end)
    :
    kernel::FunctionBase<real_t, InputTp...>(),
    function_(&function),
    lambda1_(lambda1),
    lambda2_(lambda2),
    coeff_start_(coeff_start),
    coeff_end_(coeff_end)
{}

template<typename FunctionTp, typename... InputTp>
typename ElasticNetFunction<FunctionTp, InputTp...>::output_t
ElasticNetFunction<FunctionTp, InputTp...>::value(const InputTp&...  /*input*/)const{

    auto coeffs = function_->coeffs();

    real_t sum1 = 0.0;
    real_t sum2 = 0.0;

    for(int c=coeff_start_; c<coeff_end_; ++c){

        real_t coeff = coeffs[c];
        sum1 += std::fabs(coeff);
        sum2 += coeff*coeff;
    }

    return lambda1_*sum1 + lambda2_*sum2;
}

template<typename FunctionTp, typename...InputTp>
DynVec<real_t>
ElasticNetFunction<FunctionTp, InputTp...>::gradients(const InputTp&...)const{

#ifdef USE_LOG
    kernel::Logger::log_warning("ElasticNetFunction<FunctionTp>::gradients should not be called");
#endif

    return DynVec<real_t>();
}
}

#endif // ELASTIC_NET_FUNCTION_H
