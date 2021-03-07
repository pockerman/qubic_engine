#ifndef INTEGRATOR_BASE_H
#define INTEGRATOR_BASE_H

#include "kernel/base/types.h"

#include "boost/noncopyable.hpp"
#include <vector>

namespace kernel
{

/**
  * Base class for deriving ODE integrators
  *
  */
template<typename T, typename RHS_Tp>
class ODEIntBase: private boost::noncopyable
{
public:

    typedef T value_type;
    typedef RHS_Tp rhs_type;
    typedef value_type input_type;

    /// \brief Destructor
    virtual ~ODEIntBase(){}

    /// \brief Integrate
    virtual void integrate(const input_type* input)=0;

    /// \brief Integrate for nsteps
    virtual void integrate(uint_t nsteps, const input_type** input);

    /// \brief Returns the time step
    real_t get_time_step()const{return dt_;}

    /// \brief Returns the size of the histroy vector
    uint_t history_size()const{return  static_cast<uint_t>(old_olutions_.size());}

protected:

    /// \brief Protected constructor
    ODEIntBase(value_type& val, real_t dt, uint_t history_size);

    /// \brief Pointer to the quantity the integrator
    /// should integrate
    value_type* val_;

    /// \brief The functor handles the computation of
    /// the RHS
    rhs_type rhs_;

    /// \brief The time step
    real_t dt_;

    /// \brief The old solution vectors
    std::vector<value_type> old_olutions_;

    /// \brief Returns the i-th history value
    const value_type& get_history(uint_t i)const;

    /// \brief Returns the i-th history value
    value_type& get_history(uint_t i);

    /// \brief Apply the RHS
    virtual value_type apply_rhs(const input_type* input);
};

template<typename T, typename RHS_Tp>
ODEIntBase<T, RHS_Tp>::ODEIntBase(typename ODEIntBase<T, RHS_Tp>::value_type& val, real_t dt, uint_t history_size)
    :
    val_(&val),
    rhs_(),
    dt_(dt),
    old_olutions_(history_size, typename ODEIntBase<T, RHS_Tp>::value_type())
{}

template<typename T, typename RHS_Tp>
void
ODEIntBase<T, RHS_Tp>::integrate(uint_t nsteps, const input_type** input){

    for(uint_t step=0; step<nsteps; ++step){
        integrate(input[step]);
    }
}

template<typename T, typename RHS_Tp>
const typename ODEIntBase<T, RHS_Tp>::value_type&
ODEIntBase<T, RHS_Tp>::get_history(uint_t i)const{

return old_olutions_[i];
}

template<typename T, typename RHS_Tp>
typename ODEIntBase<T, RHS_Tp>::value_type&
ODEIntBase<T, RHS_Tp>::get_history(uint_t i){

return old_olutions_[i];
}

template<typename T, typename RHS_Tp>
typename ODEIntBase<T, RHS_Tp>::value_type
ODEIntBase<T, RHS_Tp>::apply_rhs(const input_type* input){

   return rhs_(dt_, old_olutions_, input);
}


}

#endif // INTEGRATOR_BASE_H
