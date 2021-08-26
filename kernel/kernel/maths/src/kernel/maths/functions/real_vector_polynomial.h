#ifndef REAL_VECTOR_POLYNOMIAL_H
#define REAL_VECTOR_POLYNOMIAL_H

#include "kernel/base/config.h"
#include "kernel/maths/functions/function_base.h"
#include "kernel/maths/functions/monomial.h"

#include <vector>

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

namespace kernel
{

///
/// \brief The RealVectorPolynomialFunction class.
/// Models a function of the form f(x) = a0 + a1x_1^b_1+...aNx_N^b_N
///
class PolynomialFunction: public FunctionBase<real_t, DynVec<real_t> >
{

public:

    typedef DynVec<real_t> input_t;
    typedef FunctionBase<real_t, DynVec<real_t>>::output_t output_t;

    ///
    /// \brief Constructor
    ///
    PolynomialFunction();

    ///
    /// \brief Constructor Create a polynomial where
    /// each variable has order 1
    ///
    PolynomialFunction(const std::vector<real_t>& coeffs);

    ///
    /// \brief Constructor
    ///
    PolynomialFunction(const DynVec<real_t>& coeffs, const std::vector<int>& order);

    ///
    /// \brief Build the function from the given coeffs and orders
    template<typename ContainerTp>
    void create_from(const ContainerTp& coeffs, const std::vector<int>& order);

    ///
    /// \brief Returns the value of the function
    ///
    virtual output_t value(const input_t& input)const override final;

    output_t value_at(const input_t& input)const{return value(input);}

    ///
    /// \brief value
    ///
    virtual output_t value(uint_t i, const input_t&  input )const override final;

    ///
    /// \brief Returns the number of coefficients
    ///
    virtual uint_t n_coeffs()const final{return monomials_.size();}

    ///
    /// \brief Returns the gradient of the function for the i-th coefficient
    ///
    real_t coeff_grad(uint_t i, const DynVec<real_t>& point)const;

    ///
    /// \brief Returns the gradient of the function for the i-th coefficient
    ///
    DynVec<real_t> coeff_grads(const DynVec<real_t>& point)const;


    DynVec<real_t> param_grads_at(const DynVec<real_t>& point)const{return coeff_grads(point);}

    ///
    /// \brief Returns the gradient of the function for the i-th variable
    ///
    real_t grad(uint_t i, const DynVec<real_t>& point)const;

    ///
    /// \brief Returns the gradients of the function
    ///
    virtual DynVec<real_t> gradients(const DynVec<real_t>& point)const override final;

    ///
    /// \brief Returns the coefficients of the underlying monomials
    ///
    DynVec<real_t> coeffs()const override final;

    ///
    /// \brief Returns the coefficients of the underlying monomials
    ///
    template<typename ContainerTp>
    void set_coeffs(const ContainerTp& coeffs);

    ///
    /// \brief update_coeffs
    /// \param params
    ///
    virtual void update_coeffs(const DynVec<real_t>& params) override final{set_coeffs(params);}

    ///
    /// \brief Returns the i-th coefficient
    ///
    real_t coeff(uint_t c)const{return monomials_[c].coeff();}

private:

    ///
    /// \brief The monomials representing the polynomial
    ///
    std::vector<Monomial> monomials_;

};

template<typename ContainerTp>
void
PolynomialFunction::create_from(const ContainerTp& coeffs, const std::vector<int>& order){


    if(coeffs.size() != order.size()){
        throw std::invalid_argument("Coeffs size: " + std::to_string(coeffs.size())+
                                    " not equal to order size: " + std::to_string(order.size()) );
    }

    monomials_.resize((coeffs.size()));
    auto coeff_itr = coeffs.begin();
    auto order_itr = order.begin();

    for(auto& monomial : monomials_){
        monomial.create_from(*coeff_itr, *order_itr);
        coeff_itr++;
        order_itr++;
    }
}

template<typename ContainerTp>
void
PolynomialFunction::set_coeffs(const ContainerTp& coeffs){

#ifdef KERNEL_DEBUG

    assert(coeffs.size() != 0 && "Empty coefficients");

    if(monomials_.size() != 0)
        assert(monomials_.size() != coeffs.size() && "Coeffs size does not not match monomials");
#endif

    if(monomials_.size() == 0){
        monomials_.resize(coeffs.size());
    }

    auto itr = coeffs.cbegin();

    for(auto& monomial : monomials_){
        monomial.set_coeff(*itr);
        itr++;
    }
}

}

#endif // REAL_VECTOR_POLYNOMIAL_H
