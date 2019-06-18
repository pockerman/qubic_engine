/***
 * Implementation of Extended Kalman Filter algorithm
 *
 */

#ifndef EXTENDED_KALMAN_FILTER_H
#define	EXTENDED_KALMAN_FILTER_H

#include "cubic_engine/estimation/kalman_filter.h"
#include <memory>

namespace cengine
{


//TODO: The following don't look nice!!!!

/***
 * Helper class for the computation of f
 *
 **/
struct EKF_F_func
{
    /// \brief By deafult computes x = A*x_prev + B*u
    virtual void operator()(DynVec<real_t>& x, const DynVec<real_t>& x_prev, const DynMat<real_t>& A,
                            const DynMat<real_t>& B, const DynVec<real_t>& u  )const;
};

/***
 * Helper class for the computation of h
 *
 **/
struct EKF_H_func
{
    /// \brief By deafult it returns H*x
    virtual DynVec<real_t> operator()(const DynVec<real_t>& x, const DynMat<real_t>& H)const;
};
    
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

   /// \brief Constructor
   explicit ExtendedKalmanFilter(DynVec<real_t>* x=nullptr);

   /// \brief Destructor
   ~ExtendedKalmanFilter();

   /// \brief Predicts the state vector x and the process covariance matrix P using
   /// the given input control u accroding to the following rules
   ///
   ///
   /// X_{k}^{-} = f(A_{k-1}, X_{k-1}, B_k,  U_k)
   /// P_{k}^{-} = A_{k-1}P_{k-1}A_{k-1}^T + L_{k-1}Q_{k-1}L_{k-1}^T
   virtual void predict(const DynVec<real_t>& u)override final;

   /// \brief Updates the gain matrix K, the  state vector x and covariance matrix P using the given measurement y_k
   /// according to the following update rules
   ///
   /// K_k = P_{k}^{-} x H_{k}^T x (H_k x P_k{-} x H_{k}^T + M_k x R_k x M_k^T)^{-1}
   /// \hat{X}_k = X_{k}^{-} + K_k(y_k - h( X_{k}^{-}, 0)
   /// \hat{P}_k = (I - K_kH_k)P_{k}^{-}
   virtual void update(const DynVec<real_t>& y)override final;

   /// \brief Set the motion model
   void set_motion_model(EKF_F_func& func){f_ptr_ = &func;}

   /// \brief Set the observation model
   void set_observation_model(EKF_H_func& func){h_ptr_ = &func;}
           
protected:

   /// \brief pointer to the function that computes f
   EKF_F_func* f_ptr_;

   /// \brief pointer to the function that computes h
   EKF_H_func* h_ptr_;

   virtual void assert_matrix_name_(const std::string& name)const;

};  

}

#endif	/* EXTENDED_KALMAN_FILTER_H */

