#ifndef MONOMIAL_H
#define MONOMIAL_H

#include "kernel/base/types.h"
#include "kernel/maths/functions/function_base.h"
#include <cmath>
namespace kernel
{

/**
 * Represents a function of the form f(x) = a*x^k
 */
class Monomial: public FunctionBase<real_t, real_t>
{

public:

    typedef real_t input_t;
    typedef FunctionBase<real_t, real_t>::output_t output_t;

    /**
      * Constructor
      */
    Monomial();

    /**
      * Constructor
      */
    Monomial(real_t coeff, int order);

    /**
     * Returns the value of the function
     */
    virtual output_t value(const input_t& input)const override final;

    /**
      * Returns the gradients of the function
      */
    virtual DynVec<real_t> gradients(const input_t&  input)const override final{return DynVec<real_t>(1, coeff_*order_*std::pow(input, order_-1));}

    /**
      * Returns the gradient with respect to the coefficient
      */
    real_t coeff_grad(const input_t& input)const{ return std::pow(input, order_);}

     /**
       * Returns the number of coefficients
       */
    virtual uint_t n_coeffs()const final{return 1;}

     /**
       * create the monomial from the given data
       */
    void create_from(const real_t coeffs, const int order);

private:

     /**
       * The coefficient
       */
     real_t coeff_;

     /**
       * The order of the monomial
       */
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
