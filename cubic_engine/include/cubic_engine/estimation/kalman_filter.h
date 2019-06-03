/**
 * Linear Kalman Filter implementation. See
 * An Introduction to the Kalman Filter, TR 95-041
 * by
 * Greg Welch1 and Gary Bishop
 */

#include "cubic_engine/base/cubic_engine_types.h"

#include <map>
#include <string>

namespace cengine
{

/// \detailed Implements the Linear Kalman Filter algorithm. The following algorithm is
/// implemented
/// x_k = A_kx_{k-1} + B_k u_k + w_{k-1}
/// y_k = H_k x_k + v_k
///
/// where w_k and v_k  represent process and measurement noise respectively. They are assumed
/// independent and normally distributed:
///
/// p(w) ~ N(0,Q)
/// p(v) ~ N(0,R)
///
/// The following matrices dimensions are assumed:
///
/// A n x n
/// B n x l
/// u l x 1
/// H m x n
/// y m x 1
/// x n x 1
///
/// The Kalman Filter algorithm is a predictor-corrector process:
///
/// Prediction step
/// ---------------
///
/// X_{k}^{-} = A_{k-1}X_{k-1} + B_k U_k
/// P_{k}^{-} = A_{k-1}P_{k-1}A_{k-1}^T + Q_{k-1} // Process covariance prediction
///
/// Update/correct step
/// ---------------
///
/// K_k = P_{k}^{-}H_{k}^T(H_kP_k{-}H_{k}^T + R_k)^{-1}// Gain matrix it says how much the predictions should be corrected
/// \hat{X}_k = X_{k}^{-} + K_k(y_k - H_kX_{k}^{-})
/// \hat{P}_k = (I - K_kH_k)P_{k}^{-}
///
/// where
///
/// K n x m



class KalmanFilter
{

    public:

        /// \brief Constructor
        KalmanFilter();

        /// \brief Destructor
        virtual ~KalmanFilter();

        /// \brief Predict system state and process covariance matrix
        /// using the given input
        virtual void predict(const DynVec<real_t>& u);

        /// \brief Correct the predicted estimate by using the given measurement
        virtual void update(const DynVec<real_t>& y);

        /// \brief Perform one iteration i.e. predict + update of the linear Kalman filter
        void iterate(const DynVec<real_t>& u, const DynVec<real_t>& y);

        /// \brief Returns the matrices names
        std::vector<std::string> get_mat_names()const;

        /// \brief Set the pointer matrix
        void set_mat_ptr(const std::string& name, const DynMat<real_t>& mat);

    protected:

        /// \brief Pointers to the matrices describing the system
        std::map<std::string, const DynMat<real_t>* > system_maps_;

        /// The Kalman gain calculated during the course of the update step
        DynMat<real_t> K_;
};

inline
void KalmanFilter::iterate(const DynVec<real_t> &u, const DynVec<real_t> &y){

    predict(u);
    update(y);
}

inline
void
KalmanFilter::set_mat_ptr(const std::string& name, const DynMat<real_t>& mat){
    system_maps_[name] = &mat;
}
    
}
