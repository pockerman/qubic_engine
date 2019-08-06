#ifndef RUNGE_KUTTA_4_H
#define RUNGE_KUTTA_4_H

#include "cubic_engine/integrators/integrator_base.h"

namespace cengine {

/**
 * Implementation of RungeKutta 4 integrator
 */

template<typename T, typename RHS_Tp>
class RungeKutta4Int: public ODEIntBase<T, RHS_Tp>
{

public:

    typedef typename ODEIntBase<T, RHS_Tp>::rhs_type rhs_type;
    typedef typename ODEIntBase<T, RHS_Tp>::value_type value_type;
    typedef typename ODEIntBase<T, RHS_Tp>::input_type input_type;

    /// \brief Constructor
    RungeKutta4Int(value_type& val, real_t dt);

    /// \brief Destructor
    ~RungeKutta4Int()=default;

    /// \brief Integrate
    virtual void integrate(const input_type* input)override final;
};

template<typename T, typename RHS_Tp>
RungeKutta4Int<T, RHS_Tp>::RungeKutta4Int(value_type& val, real_t dt)
    :
ODEIntBase<T, RHS_Tp>(val, dt, 1)
{}

template<typename T, typename RHS_Tp>
void
RungeKutta4Int<T, RHS_Tp>::integrate(const input_type* input){


    auto tn = input[0];
    auto dt = this->get_time_step();
    auto& yn = this->get_history(0);
    auto kn1 = this->rhs_(tn, yn);
    auto kn2 = this->rhs_(tn + 0.5*dt, yn + 0.5*dt*kn1);
    auto kn3 = this->rhs_(tn + 0.5*dt, yn + 0.5*dt*kn2);
    auto kn4 = f(tn + dt, yn + dt*kn3);

    *this->val_ = yn + dt*((kn1 + 2*kn2 +2.0*kn3 + kn4)/6.0);
    yn = *this->val_;
}

template<typename RHS_Tp>
using ScalarRungeKutta4Int = RungeKutta4Int<real_t, RHS_Tp>;

template<typename RHS_Tp>
using VectorRungeKutta4Int = RungeKutta4Int<DynVec<real_t>, RHS_Tp>;


}

#endif // RUNGE_KUTTA_4_H
