/* 
 * File:   changeable_non_linear_system.h
 * Author: david
 *
 * Created on November 12, 2016, 10:40 AM
 */

#ifndef CHANGEABLE_NON_LINEAR_SYSTEM_H
#define	CHANGEABLE_NON_LINEAR_SYSTEM_H

#include "robosim/systems/system_base.h"

namespace robosim
{
    
    
/**
 * @brief A class that represents a non-linear system
 * where the state transition matrix e.t.c may change
 * during time
 */
class NonLinSys: public SystemBase
{
    
protected:
    
    /**
     * @brief Constructor.
     */
    NonLinSys();
    
public:
    
    /**
     * @brief Destructor
     */
    virtual ~NonLinSys();
    
    /**
     * @brief Make any updates to the system
     */
    virtual void update_AWQ_mats()=0;
    
    /**
     * @brief Update the state vector
     */
    virtual void update_x_vec()=0;
    
    
    /**
     * @brief Update the measurements vector
     */
    virtual void update_z_vec()=0;
    
    
    /**
     * @brief Update the H, V, R matrices of the system
     */
    virtual void update_HVR_mats()=0;
        
}; 
    
}



#endif	/* CHANGEABLE_NON_LINEAR_SYSTEM_H */

