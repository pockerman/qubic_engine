/**
 * Linear Kalman Filter implementation. See
 * An Introduction to the Kalman Filter, TR 95-041
 * by
 * Greg Welch1 and Gary Bishop
 */

#include "cubic_engine/base/cubic_engine_types.h"
#include <boost/noncopyable.hpp>

#include <map>
#include <string>
#include <stdexcept> //for std::invalid_argument


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
///
/// As the class manipulates a a given state vector it does not make  sense to
/// be copyable
class KalmanFilter: private boost::noncopyable
{
    public:

        /// \brief Constructor
        explicit KalmanFilter(DynVec<real_t>* x=nullptr);

        /// \brief Destructor
        virtual ~KalmanFilter();

        /// \brief Predicts the state and the process covariance matrix using
        /// X_{k}^{-} = A_{k-1}X_{k-1} + B_k U_k
        /// P_{k}^{-} = A_{k-1}P_{k-1}A_{k-1}^T + Q_{k-1}
        /// u: The control input
        virtual void predict(const DynVec<real_t>& u);

       /// \brief Updates the state and covariance matrices using the measurement
       ///
       ///K_k = P_{k}^{-}H_{k}^T(H_kP_k{-}H_{k}^T + R_k)^{-1}
       ///\hat{X}_k = X_{k}^{-} + K_k(y_k - H_kX_{k}^{-})
       ///\hat{P}_k = (I - K_kH_k)P_{k}^{-}
        virtual void update(const DynVec<real_t>& y);

        /// \brief Perform one iteration i.e. predict + update of the linear Kalman filter
        void iterate(const DynVec<real_t>& u, const DynVec<real_t>& y);

        /// \brief Returns the matrices names
        std::vector<std::string> get_mat_names()const;

        /// \brief Set the pointer matrix
        void set_mat_ptr(const std::string& name, DynMat<real_t>& mat);

        /// \brief Set the pointer to the state vector the filter manipulates
        void set_state_vector_ptr(DynVec<real_t>& x ){x_ = &x;}

    protected:

        /// \brief Pointers to the matrices describing the system
        std::map<std::string, DynMat<real_t>* > system_maps_;

        /// The Kalman gain calculated during the course of the update step
        DynMat<real_t> K_;

        /// \brief The state vector that the filter manipulates
        DynVec<real_t>* x_;


        virtual void assert_matrix_name_(const std::string& name)const;

};

inline
void KalmanFilter::iterate(const DynVec<real_t> &u, const DynVec<real_t> &y){

    predict(u);
    update(y);
}

inline
void
KalmanFilter::set_mat_ptr(const std::string& name, DynMat<real_t>& mat){

    assert_matrix_name_(name);
    system_maps_[name] = &mat;
}
    
}
