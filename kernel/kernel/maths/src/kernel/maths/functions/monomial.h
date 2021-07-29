#ifndef MONOMIAL_H
#define MONOMIAL_H

#include "kernel/base/types.h"
#include "kernel/maths/functions/function_base.h"

#include <cmath>


namespace kernel
{

///
/// \brief Represents a function of the form f(x) = a*x^k
///
class Monomial: public FunctionBase<real_t, real_t>
{

public:

    typedef real_t input_t;
    typedef FunctionBase<real_t, real_t>::output_t output_t;

    ///
    /// \brief Constructor
    ///
    Monomial();

    ///
    /// \brief Constructor
    ///
    Monomial(real_t coeff, int order);

    ///
    /// \brief Returns the value of the function
    ///
    virtual output_t value(const input_t& input)const override final;

    ///
    /// \brief gradient/ Returns the o-th order gradient of the monomial
    /// at point p
    ///
    real_t gradient(real_t p, uint_t o)const;

    ///
    /// \brief Returns the gradients of the function
    ///
    virtual DynVec<real_t> gradients(const input_t&  input)const override final
    {return DynVec<real_t>(1, coeff_*order_*std::pow(input, order_-1));}

    ///
    /// \brief Returns the gradient with respect to the coefficient
    ///
    real_t coeff_grad(const input_t& input)const{ return std::pow(input, order_);}

    ///
    /// \brief Returns the number of coefficients
    ///
    virtual uint_t n_coeffs()const final{return 1;}

    ///
    /// \brief Create the monomial from the given data
    ///
    void create_from(const real_t coeffs, const int order);

    ///
    /// \brief Returns the coefficient
    ///
    real_t coeff()const{return coeff_;}

    ///
    /// \brief Set the coefficient
    ///
    void set_coeff(real_t coeff){coeff_ = coeff;}

    ///
    /// \brief coeffs
    /// \return
    ///
    virtual DynVec<real_t> coeffs()const override final{return DynVec<real_t>(1, coeff_);}

    ///
    /// \brief update_coeffs
    /// \param params
    ///
    virtual void update_coeffs(const DynVec<real_t>& params) override final{coeff_ = params[0];}

private:

     ///
     /// \brief The coefficient
     ///
     real_t coeff_;

     ///
     /// \brief The order of the monomial
     ///
     int order_;
};

inline
Monomial::Monomial()
    :
      FunctionBase<real_t, real_t>(),
      coeff_(1.0),
      order_(0)
{}

inline
Monomial::Monomial(real_t coeff, int order)
    :
      FunctionBase<real_t, real_t>(),
      coeff_(coeff),
      order_(order)
{}

inline
Monomial::output_t
Monomial::value(const Monomial::input_t& input)const{
    return coeff_*std::pow(input, order_);
}

inline
void
Monomial::create_from(const real_t coeff, const int order){
    coeff_ = coeff;
    order_ = order;
}

}

#endif // MONOMIAL_H
