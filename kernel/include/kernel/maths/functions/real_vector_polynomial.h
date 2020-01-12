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

    /// \brief Constructor
    RealVectorPolynomialFunction();

    /// \brief Constructor Create a polynomial where each variable has order 1
    RealVectorPolynomialFunction(const std::vector<real_t>& coeffs);

    /// \brief Constructor
    RealVectorPolynomialFunction(const DynVec<real_t>& coeffs, const std::vector<int>& order);

    /// \brief Build the function from the given coeffs and orders
    void create_from(const DynVec<real_t>& coeffs, const std::vector<int>& order);

    /**
      * Create a polynomial where each variable has order 1
      */
    void create_from(const std::vector<real_t>& coeffs);

    /**
     * Returns the value of the function
     */
    virtual output_t value(const input_t& input)const override final;

    /**
      * Returns the number of coefficients
      */
    virtual uint_t n_coeffs()const final{return monomials_.size();}

    /**
      * Returns the gradient of the function for the i-th coefficient
      */
    virtual real_t coeff_grad(uint_t i, const DynVec<real_t>& point)const override final;

    /**
      * Returns the gradient of the function for the i-th coefficient
      */
    DynVec<real_t> coeff_grads(const DynVec<real_t>& point)const;

    /**
      * Returns the gradient of the function for the i-th variable
      */
    virtual real_t grad(uint_t i, const DynVec<real_t>& point)const override final;

    /**
      * Returns the gradients of the function
      */
    virtual DynVec<real_t> gradients(const DynVec<real_t>& point)const override final;

    /// \brief Returns the coefficients of the underlying monomials
    std::vector<real_t> coeffs()const;

    /**
      * Returns the coefficients of the underlying monomials
      */
    void set_coeffs(const std::vector<real_t>& coeffs);

    /// \brief Returns the i-th coefficient
    real_t coeff(uint_t c)const{return monomials_[c].coeff();}


private:

    /**
     * The monomials representing the polynomial
     */
    std::vector<Monomial> monomials_;

};

}

#endif // REAL_VECTOR_POLYNOMIAL_H
