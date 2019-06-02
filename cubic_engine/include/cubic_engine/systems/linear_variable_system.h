/* 
 * File:   linear_variable_system.h
 * Author: david
 *
 * Created on November 19, 2016, 1:03 PM
 */

#ifndef LINEAR_VARIABLE_SYSTEM_H
#define	LINEAR_VARIABLE_SYSTEM_H

#include "robosim/systems/linear_steady_system.h"

namespace robosim
{

/**
 * @brief A class that models a linear system of the form
 * 
 * $$\mathbf{x}_k = \matbf{A}_{k-1}\mathbf{x}_{k-1} + \mathbf{B}_{k-1}\mathbf{u}_{t-1}+ \mathbf{W}_{k-1}\mathbf{w}_{k-1}$$
 * 
 * $$\mathbf{z}_k = \mathbf{H}_k\mathbf{x}_k +  \mathbf{v}_k$$.
 * 
 * where \f$k\f$ is the discrete time step index. 
 * For such a system the involved coefficients matrices are not assumed constant throughout the
 * time iteration. This class simply exposes a virtual function that concrete classes should
 * override in order to implement the updates.
 * 
 * http://kalman.sourceforge.net/doc/a00012.html#_details
 */
class LinearVariableSys: public LinearSteadySys
{
    
public:
    
    /**
     * @brief Expose the type of the matrix the system uses
     */
    typedef LinearSteadySys::mat_type mat_type;
    
    /**
     * @brief Expose the type of the vector the system uses
     */
    typedef LinearSteadySys::vec_type vec_type;
    
protected:
    
    /**
     * @brief Constructor
     */
    LinearVariableSys();
    
public:
    
    /**
     * @brief Destructor
     */
    virtual ~LinearVariableSys();
    
    
    /**
     * @brief Update the coefficient matrices for the system
     */
    virtual void update_mats()=0;
    
    
    
    
};    
    
}



#endif	/* LINEAR_VARIABLE_SYSTEM_H */

