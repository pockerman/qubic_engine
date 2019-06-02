/* 
 * File:   extended_kalman_filter.h
 * Author: david
 *
 * Created on October 7, 2017, 10:35 AM
 */

#ifndef EXTENDED_KALMAN_FILTER_H
#define	EXTENDED_KALMAN_FILTER_H

#include "kernel/memory/observer_ptr.h"

namespace robosim
{
    
/**
 * @brief Naive in the sense of MRPT Extended Kalman filter algorithm.
 * The type of the system is passed as a template parameter
 */
template<typename SystemTp> 
class NaiveEKF
{
    public:
        
        /**
         * @brief The type of the system that uses the filter
         */
        typedef SystemTp agent_type;
        
        /**
         * @brief Constructor
         */
        NaiveEKF(SystemTp& sys);
        
        /**
         * @brief Execute the filter. This is simply
         * one iteration of the filter
         */
        void execute();
        
        
    protected:
        
        
        /**
         * @brief The algorithm may need to change the
         * system
         */
        kernel::observer_ptr<SystemTp> sys_ptr_;    
};  

template<typename SystemTp>
NaiveEKF<SystemTp>::NaiveEKF(SystemTp& sys)
:
sys_ptr_(&sys)
{}


template<typename SystemTp>
void
NaiveEKF<SystemTp>::execute(){
    
    
}



}

#endif	/* EXTENDED_KALMAN_FILTER_H */

