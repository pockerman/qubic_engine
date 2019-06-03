/***
 * Implementation of Extended Kalman Filter algorithm
 *
 */

#ifndef EXTENDED_KALMAN_FILTER_H
#define	EXTENDED_KALMAN_FILTER_H

#include "cubic_engine/estimation/kalman_filter.h"

namespace cengine
{
    
/***
 *  Implements the Extended Kalman filter algorithm. This class inherits from KalmanFilter class
 *  and overrides the predict and update functions. In terms of implementation, the following differences
 *  are noted
 *
 *  Prediction step
 *  ---------------
 *  1. The predicted state vector \check{x} is computed via an application defined function specified by the 'f' keyword
 *  2. The computation of predicted process covariance matrix \check{P} is augmented with the L matrix as follows
 *          P_{k}^{-} = A_{k-1}P_{k-1}A_{k-1}^T + L_{k-1}Q_{k-1}L_{k-1}^T
 *  The linear case does not involve L
 *
 *
 *  Update/correct step
 *  ---------------
 *
 *  1. The computation of the Kalman gain matrix K, is augmented with an M matrix as follows
 *
 *          K_k = P_{k}^{-} x H_{k}^T x (H_k x P_k{-} x H_{k}^T + M_k x R_k x M_k^T)^{-1}
 *      The application should set this matrix using the M keyword
 *
 *  2. The computation of the innovation uses an application defined function specified by the 'h' keyword.
 */

class ExtendedKalmanFilter: public KalmanFilter
{
public:


   ExtendedKalmanFilter();

   ~ExtendedKalmanFilter();

   /// \brief Predict system state and process covariance matrix
   /// using the given input
   virtual void predict(const DynVec<real_t>& u)override final;

   /// \brief Correct the predicted estimate by using the given measurement
   virtual void update(const DynVec<real_t>& y)override final;
           
protected:
        

};  

}

#endif	/* EXTENDED_KALMAN_FILTER_H */

