#ifndef EXTENDED_KALMAN_FILTER_H
#define	EXTENDED_KALMAN_FILTER_H

#include "cubic_engine/base/cubic_engine_types.h"

#include <map>
#include <string>

namespace cengine{


///
/// Implements the Extended Kalman filter algorithm. This class inherits from KalmanFilter class
/// and overrides the predict and update functions. In terms of implementation, the following differences
/// are noted
///
/// Prediction step
/// ---------------
/// 1. The predicted state vector \check{x} is computed via an application defined function specified by the 'f' keyword
/// 2. The computation of predicted process covariance matrix \check{P} is augmented with the L matrix as follows
///         P_{k}^{-} = A_{k-1}P_{k-1}A_{k-1}^T + L_{k-1}Q_{k-1}L_{k-1}^T
/// The linear case does not involve L
///
///
/// Update/correct step
/// ---------------
///
/// 1. The computation of the Kalman gain matrix K, is augmented with an M matrix as follows
///
///         K_k = P_{k}^{-} x H_{k}^T x (H_k x P_k{-} x H_{k}^T + M_k x R_k x M_k^T)^{-1}
///     The application should set this matrix using the M keyword
///
/// 2. The computation of the innovation uses an application defined function specified by the 'h' keyword.
///

template<typename MotionModelTp, typename ObservationModelTp>
class ExtendedKalmanFilter
{
public:

    typedef MotionModelTp motion_model_t;
    typedef ObservationModelTp observation_model_t;
    typedef typename motion_model_t::input_t motion_model_input_t;
    typedef typename motion_model_t::matrix_t matrix_t;
    typedef typename observation_model_t::input_t observation_model_input_t;

    /// \brief Constructor
    ExtendedKalmanFilter();

    /// \brief Constructor
    ExtendedKalmanFilter(motion_model_t& motion_model, const observation_model_t& observation_model);

    /// \brief Destructor
    ~ExtendedKalmanFilter();

   /// \brief Predicts the state vector x and the process covariance matrix P using
   /// the given input control u accroding to the following rules
   ///
   ///
   /// X_{k}^{-} = f(A_{k-1}, X_{k-1}, B_k,  U_k)
   /// P_{k}^{-} = A_{k-1}P_{k-1}A_{k-1}^T + L_{k-1}Q_{k-1}L_{k-1}^T
   virtual void predict(const motion_model_input_t& input);

   /// \brief Updates the gain matrix K, the  state vector x and covariance matrix P using the given measurement y_k
   /// according to the following update rules
   ///
   /// K_k = P_{k}^{-} x H_{k}^T x (H_k x P_k{-} x H_{k}^T + M_k x R_k x M_k^T)^{-1}
   /// \hat{X}_k = X_{k}^{-} + K_k(y_k - h( X_{k}^{-}, 0)
   /// \hat{P}_k = (I - K_kH_k)P_{k}^{-}
   virtual void update(const observation_model_input_t& z);

   /// \brief Set the motion model
   void set_motion_model(motion_model_t& motion_model){motion_model_ptr_ = &motion_model;}

   /// \brief Set the observation model
   void set_observation_model(const observation_model_t& observation_model){observation_model_ = &observation_model;}

   /// \brief Set the matrix used by the filter
   void set_matrix(const std::string& name, const matrix_t& mat);
           
protected:

   /// \brief pointer to the function that computes f
   motion_model_t* motion_model_ptr_;

   /// \brief pointer to the function that computes h
   const observation_model_t* observation_model_;

   /// \brief Matrices used by the filter internally
   std::map<std::string, matrix_t> matrices_;

};  

template<typename MotionModelTp, typename ObservationModelTp>
ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::ExtendedKalmanFilter()
    :
    motion_model_ptr_(nullptr),
    observation_model_(nullptr)
{}

template<typename MotionModelTp, typename ObservationModelTp>
ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::ExtendedKalmanFilter(typename ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::motion_model_t& motion_model,
                                                                             const typename ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::observation_model_t& observation_model)
    :
    motion_model_ptr_(&motion_model),
    observation_model_(&observation_model)
{}

template<typename MotionModelTp, typename ObservationModelTp>
ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::~ExtendedKalmanFilter()
{}

template<typename MotionModelTp, typename ObservationModelTp>
void
ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::set_matrix(const std::string& name, const matrix_t& mat){

    if(name != "Q" && name != "K" && name != "R" && name != "P"){
        throw std::logic_error("Invalid matrix name. Name: "+
                               name+
                               " not in [Q, K, R, P]");
    }

    matrices_.insert_or_assign(name, mat);
}

template<typename MotionModelTp, typename ObservationModelTp>
void
ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::predict(const typename ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::motion_model_input_t& u){

    // make a state predicion using the
    // motion model
    auto& state = motion_model_ptr_->evaluate(u);

    auto& P = matrices_["P"];
    auto& Q = matrices_["Q"];

    auto& L = motion_model_ptr_->get_matrix("L");
    auto L_T = trans(L);

    auto& F = motion_model_ptr_->get_matrix("F");
    auto F_T = trans(F);

    P = F * P * F_T; // + L*Q*L_T;
}

template<typename MotionModelTp, typename ObservationModelTp>
void
ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::update(const typename ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::observation_model_input_t&  z){

    auto& state = motion_model_ptr_->get_state();
    auto& P = matrices_["P"];
    auto& R = matrices_["R"];

    auto zpred = observation_model_->evaluate(z);

    auto& H = observation_model_->get_matrix("H");
    auto H_T = trans(H);

    // compute \partial{h}/\partial{v} the jacobian of the observation model
    // w.r.t the error vector
    auto& M = observation_model_->get_matrix("M");
    auto M_T = trans(M);
    auto& K = matrices_["K"];

     try{

        // S = H*P*H^T + M*R*M^T
        auto S = H*P*H_T + M*R*M_T;

        auto S_inv = inv(S);
        K = P*H_T*S_inv;

        auto innovation = z - zpred;

        if(K.columns() != innovation.size()){
            throw std::runtime_error("Matrix columns: "+std::to_string(K.columns())+" not equal to vector size: "+std::to_string(innovation.size()));
        }

        state.add(K*innovation);

        IdentityMatrix<real_t> I(state.size());

        // update the covariance matrix
        P =  (I - K*H)*P;
    }
    catch(...){

        //# this is a singular matrix what
        //# should we do? Simply use the predicted
        //# values and log the fact that there was a singular matrix

        throw;
    }
}


}

#endif	/* EXTENDED_KALMAN_FILTER_H */

