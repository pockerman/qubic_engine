#ifndef FWD_EULER_H
#define FWD_EULER_H

#include "kernel/integrators/integrator_base.h"

namespace kernel
{

template<typename T, typename RHS_Tp>
class FWDEulerInt: public ODEIntBase<T, RHS_Tp>
{

public:

    typedef typename ODEIntBase<T, RHS_Tp>::rhs_type rhs_type;
    typedef typename ODEIntBase<T, RHS_Tp>::value_type value_type;
    typedef typename ODEIntBase<T, RHS_Tp>::input_type input_type;

    /// \brief Constructor
    FWDEulerInt(value_type& val, real_t dt);

    /// \brief Integrate
    virtual void integrate(const input_type* input)override final;

};

template<typename T, typename RHS_Tp>
FWDEulerInt<T, RHS_Tp>::FWDEulerInt(value_type& val, real_t dt)
    :
ODEIntBase<T, RHS_Tp>(val, dt, 1)
{}

template<typename T, typename RHS_Tp>
void
FWDEulerInt<T, RHS_Tp>::integrate(const input_type* input){
    auto& old = this->get_history(0);
    *(this->val_) = old + this->get_time_step()*this->apply_rhs(input);
    old = *this->val_;
}

template<typename RHS_Tp>
using ScalarFWDEulerInt = FWDEulerInt<real_t, RHS_Tp>;

template<typename RHS_Tp>
using VectorFWDEulerInt = FWDEulerInt<DynVec<real_t>, RHS_Tp>;

}

#endif // FWD_EULER_H
