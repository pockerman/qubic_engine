#include "kernel/maths/functions/real_vector_polynomial.h"

#include <exception>
namespace kernel
{


RealVectorPolynomialFunction::RealVectorPolynomialFunction()
    :
      RealVectorValuedFunctionBase(),
      monomials_()
{}


RealVectorPolynomialFunction::RealVectorPolynomialFunction(const DynVec<real_t>& coeffs, const std::vector<int>& order)
    :
  RealVectorPolynomialFunction()
{
    this->create_from(coeffs, order);
}


void
RealVectorPolynomialFunction::create_from(const DynVec<real_t>& coeffs, const std::vector<int>& order){

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



RealVectorPolynomialFunction::output_t
RealVectorPolynomialFunction::value(const RealVectorPolynomialFunction::input_t& input){

    if(input.size() != monomials_.size()){
        throw std::invalid_argument("input size: " + std::to_string(input.size())+
                                    " not equal to monomials size: " + std::to_string(monomials_.size()) );

    }

    RealVectorPolynomialFunction::output_t result = 0.0;

    uint_t i=0;
    for(auto item : input){

        result += monomials_[i++].value(item);
    }

    return result;

}
}
