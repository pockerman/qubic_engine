/* 
 * File:   linear_system.h
 * Author: david
 *
 * Created on November 7, 2016, 12:29 PM
 */

#ifndef LINEAR_STEADY_SYSTEM_H
#define	LINEAR_STEADY_SYSTEM_H

#include "robosim/systems/system_base.h"

namespace robosim
{
 
/**
 * @brief A class that models a linear system of the form
 * 
 * $$\mathbf{x}_k = \matbf{A}\mathbf{x}_{k-1} + \mathbf{B}\mathbf{u}_{k-1}+ \mathbf{W}\mathbf{w}_{k-1}$$
 * 
 * $$\mathbf{z}_k = \mathbf{H}\mathbf{x}_k + \mathbf{v}_t$$.
 * 
 * where \f$k\f$ is the discrete time step index.
 * For such a system the involved coefficients matrices are assumed constant throughout the
 * time iteration
 * 
 * http://kalman.sourceforge.net/doc/a00012.html#_details
 */
class LinearSteadySys: public SystemBase
{
   
public:
    
    /**
     * @brief Expose the type of the matrix the system uses
     */
    typedef SystemBase::mat_type mat_type;
    
    /**
     * @brief Expose the type of the vector the system uses
     */
    typedef SystemBase::vec_type vec_type;
    
    /**
     * @brief Constructor
     */
    LinearSteadySys();
    
    
    /**
     * @brief Return a reference to the control input matrix
     */
    mat_type& get_B_mat(){return Bt_;}
    
    
     /**
     * @brief Return a reference to the control input matrix
     */
    const mat_type& get_B_mat()const{return Bt_;}
    
       
protected:
     
    /**
     *@brief The control input matrix. This an \f$n\times m\f$ matrix
     * where \f$n\f$ is the dimension of the state vector and \f$m\f$ is
     * the dimension of the control vector
     */ 
    mat_type Bt_;
   
};  


    
}



#endif	/* LINEAR_SYSTEM_H */

