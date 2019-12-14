#ifndef REAL_VECTOR_POLYNOMIAL_H
#define REAL_VECTOR_POLYNOMIAL_H

#include "kernel/maths/functions/real_vector_function_base.h"
#include "kernel/maths/functions/monomial.h"

#include <vector>
namespace kernel
{

/**
 * Models a function of the form f(x) = a0 + a1x_1^b_1+...aNx_N^b_N
 */
class RealVectorPolynomialFunction: public RealVectorValuedFunctionBase
{

public:

    typedef RealVectorValuedFunctionBase::input_t input_t;
    typedef RealVectorValuedFunctionBase::output_t output_t;

    /**
      * Constructor
      */
    RealVectorPolynomialFunction();

    /**
      * Constructor
      */
    RealVectorPolynomialFunction(const DynVec<real_t>& coeffs, const std::vector<int>& order);

    /**
      * Build the function from the given coeffs and orders
      */
    void create_from(const DynVec<real_t>& coeffs, const std::vector<int>& order);


    /**
     * Returns the value of the function
     */
    virtual output_t value(const input_t& input)final;


    /**
      * Returns the number of coefficients
      */
    virtual uint_t n_coeffs()const final{return monomials_.size();}


    /**
      * Returns the gradient of the function for the i-th coefficient
      */
    virtual real_t coeff_grad(uint_t i, const DynVec<real_t>& point)const{return 0.0;};


    /**
      * Returns the gradient of the function for the i-th variable
      */
    virtual real_t grad(uint_t i, const DynVec<real_t>& point)const{return 0.0;}


private:

    /**
     * The monomials representing the polynomial
     */
    std::vector<Monomial> monomials_;

};

}

#endif // REAL_VECTOR_POLYNOMIAL_H
