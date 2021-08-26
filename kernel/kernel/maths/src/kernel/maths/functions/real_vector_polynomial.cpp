#include "kernel/maths/functions/real_vector_polynomial.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

#include <exception>


namespace kernel
{

PolynomialFunction::PolynomialFunction()
    :
      monomials_()
{}


PolynomialFunction::PolynomialFunction(const std::vector<real_t>& coeffs)
    :
      monomials_()
{

    create_from(coeffs, std::vector<int>(coeffs.size(), 1));
}


PolynomialFunction::PolynomialFunction(const DynVec<real_t>& coeffs, const std::vector<int>& order)
    :
      monomials_()
{
    create_from(coeffs, order);
}


DynVec<real_t>
PolynomialFunction::coeffs()const{

    DynVec<real_t> coeffs(monomials_.size(), 0.0);

    for(uint_t c=0; c<monomials_.size(); ++c ){
       coeffs[c] = monomials_[c].coeff();
    }

    return coeffs;
}



PolynomialFunction::output_t
PolynomialFunction::value(const input_t& input)const{

    if(input.size() != monomials_.size()){
        throw std::invalid_argument("input size: " + std::to_string(input.size())+
                                    " not equal to monomials size: " + std::to_string(monomials_.size()) );
    }

    PolynomialFunction::output_t result = 0.0;
    uint_t i=0;
    for(auto item : input){

        result += monomials_[i++].value(item);
    }

    return result;
}


PolynomialFunction::output_t
PolynomialFunction::value(uint_t i, const input_t& input)const{

    if(i >=  monomials_.size()){
        throw std::invalid_argument("Invalid index= " + std::to_string(i)+
                                    " should be less than  " + std::to_string(monomials_.size()) );
    }

    return monomials_[i].value(input[i]);
}


real_t
PolynomialFunction::coeff_grad(uint_t i, const DynVec<real_t>& point)const{

    if(i == 0){
        return 1.0;
    }

    return monomials_[i].coeff_grad(point[i]);
}

DynVec<real_t>
PolynomialFunction::coeff_grads(const DynVec<real_t>& point)const{

    DynVec<real_t> result(monomials_.size(), 0.0);

    for(uint_t c=0; c<result.size(); ++c){
        result[c] = coeff_grad(c, point);
    }

    return result;
}

real_t
PolynomialFunction::grad(uint_t i, const DynVec<real_t>& point)const{

    if(i == 0){
        return 0.0;
    }

    return monomials_[i].gradients(point[i])[0];
}

DynVec<real_t>
PolynomialFunction::gradients(const DynVec<real_t>& point)const{

    DynVec<real_t> result(monomials_.size(), 0.0);

    for(uint_t c=0; c<result.size(); ++c){
        result[c] = grad(c, point);
    }

    return result;
}
}
