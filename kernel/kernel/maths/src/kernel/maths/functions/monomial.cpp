#include "kernel/maths/functions/monomial.h"

namespace kernel{

real_t
Monomial::gradient(real_t p, uint_t o)const{

    auto result = 0.0;

    if( o > order_){
        return result;
    }

    auto order = order_;
    auto coeff = 1.0;

    for(int i=0; i < o; ++i ){
        coeff *=order;
        order -= 1;
    }

    return coeff_*coeff*std::pow(p, order);
}


}
