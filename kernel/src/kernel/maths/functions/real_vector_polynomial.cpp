#include "kernel/maths/functions/real_vector_polynomial.h"

#include <exception>
namespace kernel
{


RealVectorPolynomialFunction::RealVectorPolynomialFunction()
    :
      RealVectorValuedFunctionBase(),
      monomials_()
{}


RealVectorPolynomialFunction::RealVectorPolynomialFunction(const std::vector<real_t>& coeffs)
    :
      RealVectorValuedFunctionBase(),
      monomials_()
{
    create_from(coeffs);
}


RealVectorPolynomialFunction::RealVectorPolynomialFunction(const DynVec<real_t>& coeffs, const std::vector<int>& order)
    :
  RealVectorPolynomialFunction()
{
    create_from(coeffs, order);
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

void
RealVectorPolynomialFunction::create_from(const std::vector<real_t>& coeffs){

    monomials_.resize((coeffs.size()));
    auto coeff_itr = coeffs.begin();

    for(auto& monomial : monomials_){
        monomial.create_from(*coeff_itr, 1);
        coeff_itr++;
    }
}


std::vector<real_t>
RealVectorPolynomialFunction::coeffs()const{

    std::vector<real_t> coeffs;
    coeffs.reserve(monomials_.size());

    for(const auto& monomial : monomials_){
        coeffs.push_back(monomial.coeff());
    }

    return coeffs;
}

void
RealVectorPolynomialFunction::set_coeffs(const std::vector<real_t>& coeffs){

    if(monomials_.size() != coeffs.size()){
        throw std::invalid_argument("Monomials size "+std::to_string(monomials_.size()) +
                                    " does not match the coeffs: "+std::to_string(coeffs.size()));
    }

    auto itr = coeffs.cbegin();

    for(auto& monomial : monomials_){
        monomial.set_coeff(*itr);
        itr++;
    }
}

void
RealVectorPolynomialFunction::set_coeffs(const DynVec<real_t>& coeffs){

    if(monomials_.size() != coeffs.size()){
        throw std::invalid_argument("Monomials size "+std::to_string(monomials_.size()) +
                                    " does not match the coeffs: "+std::to_string(coeffs.size()));
    }

    auto itr = coeffs.cbegin();

    for(auto& monomial : monomials_){
        monomial.set_coeff(*itr);
        itr++;
    }

}

RealVectorPolynomialFunction::output_t
RealVectorPolynomialFunction::value(const RealVectorPolynomialFunction::input_t& input)const{

    if(input.size() != monomials_.size()){
        throw std::invalid_argument("input size: " + std::to_string(input.size())+
                                    " not equal to monomials size: " + std::to_string(monomials_.size()) );
    }

    RealVectorPolynomialFunction::output_t result = 0.0;
    uint_t i=0;
    for(auto item : input){

        result += monomials_[i++].value(item);
    }

    /*if(result >= 0.0){
        return 1.0;
    }

    if(result < 0.0){
        return 0.0;
    }*/

    /*if(this->is_max_clipped()){
        if(result > this->get_max_clip_limit_value()){
            result = this->get_max_clip_value();
        }
    }

    if(this->is_min_clipped()){
        if(result < this->get_min_clip_limit_value()){
            result = this->get_min_clip_value();
        }
    }*/


    return result;
}


real_t
RealVectorPolynomialFunction::coeff_grad(uint_t i, const DynVec<real_t>& point)const{

    if(i == 0){
        return 1.0;
    }

    return monomials_[i].coeff_grad(point[i]);
}

DynVec<real_t>
RealVectorPolynomialFunction::coeff_grads(const DynVec<real_t>& point)const{

    DynVec<real_t> result(monomials_.size(), 0.0);

    for(uint_t c=0; c<result.size(); ++c){
        result[c] = coeff_grad(c, point);
    }

    return result;
}

real_t
RealVectorPolynomialFunction::grad(uint_t i, const DynVec<real_t>& point)const{

    if(i == 0){
        return 0.0;
    }

    return monomials_[i].gradients(point[i])[0];
}

DynVec<real_t>
RealVectorPolynomialFunction::gradients(const DynVec<real_t>& point)const{

    DynVec<real_t> result(monomials_.size(), 0.0);

    for(uint_t c=0; c<result.size(); ++c){
        result[c] = grad(c, point);
    }

    return result;
}
}
