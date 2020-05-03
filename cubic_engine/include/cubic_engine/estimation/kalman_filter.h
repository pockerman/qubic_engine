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

template<typename MotionModelTp, typename ObservationModelTp>
class KalmanFilter: private boost::noncopyable
{

public:

    typedef MotionModelTp motion_model_t;
    typedef ObservationModelTp observation_model_t;
    typedef typename motion_model_t::input_t motion_model_input_t;
    typedef typename motion_model_t::matrix_t matrix_t;
    typedef typename motion_model_t::state_t state_t;
    typedef typename observation_model_t::input_t observation_model_input_t;

    /// \brief Constructor
    KalmanFilter();

    /// \brief Constructor
    KalmanFilter(motion_model_t& motion_model, const observation_model_t& observation_model);

    /// \brief Destructor
    ~KalmanFilter();

    /// \brief Estimate the state. This function simply
    /// wraps the predict and update steps described by the
    /// functions below
    void estimate(const std::tuple<motion_model_input_t, observation_model_input_t>& input );

    /// \brief Predicts the state vector x and the process covariance matrix P using
    /// the given input control u accroding to the following equations
    ///
    /// \hat{x}_{k = f(x_{k-1}, u_{k}, w_k)
    /// \hat{P}_{k} = F_{k-1} * P_{k-1} * F_{k-1}^T + L_{k-1} * Q_{k-1} * L_{k-1}^T
    ///
    /// where x_{k-1} is the state at the previous step, u_{k}
    /// is the control signal and w_k is the error associated with the
    /// control signal. In input argument passed to the function is meant
    /// to model in a tuple all the arguments needed. F, L are Jacobian matrices
    /// and Q is the covariance matrix associate with the control signal
    ///
    /// \brief Predicts the state and the process covariance matrix using
    /// X_{k}^{-} = A_{k-1}X_{k-1} + B_k U_k
    /// P_{k}^{-} = A_{k-1}P_{k-1}A_{k-1}^T + Q_{k-1}
    /// u: The control input
    void predict(const motion_model_input_t& input);

    /// \brief Updates the gain matrix K, the  state vector x and covariance matrix P
    /// using the given measurement z_k according to the following equations
    ///
    /// K_k = \hat{P}_{k} * H_{k}^T * (H_k * \hat{P}_{k} * H_{k}^T + M_k * R_k * M_k^T)^{-1}
    /// x_k = \hat{x}_{k} + K_k * (z_k - h( \hat{x}_{k}, 0))
    /// P_k = (I - K_k * H_k) * \hat{P}_{k}
    ///
    /// \brief Updates the state and covariance matrices using the measurement
    ///
    ///K_k = P_{k}^{-}H_{k}^T(H_kP_k{-}H_{k}^T + R_k)^{-1}
    ///\hat{X}_k = X_{k}^{-} + K_k(y_k - H_kX_{k}^{-})
    ///\hat{P}_k = (I - K_kH_k)P_{k}^{-}
    void update(const observation_model_input_t& z);

    /// \brief Set the motion model
    void set_motion_model(motion_model_t& motion_model)
    {motion_model_ptr_ = &motion_model;}

    /// \brief Set the observation model
    void set_observation_model(const observation_model_t& observation_model)
    {observation_model_ptr_ = &observation_model;}

    /// \brief Set the matrix used by the filter
    void set_matrix(const std::string& name, const matrix_t& mat);

    /// \brief Returns true if the matrix with the given name exists
    bool has_matrix(const std::string& name)const;

    /// \brief Returns the state
    const state_t& get_state()const{return motion_model_ptr_->get_state();}

    /// \brief Returns the state
    state_t& get_state(){return motion_model_ptr_->get_state();}

    /// \brief Returns the state property with the given name
    real_t get(const std::string& name)const{return motion_model_ptr_->get(name);}

protected:

    /// \brief pointer to the function that computes f
    motion_model_t* motion_model_ptr_;

    /// \brief pointer to the function that computes h
    const observation_model_t* observation_model_ptr_;

    /// \brief Matrices used by the filter internally
    std::map<std::string, matrix_t> matrices_;
};

template<typename MotionModelTp, typename ObservationModelTp>
KalmanFilter<MotionModelTp,ObservationModelTp>::KalmanFilter()
    :
    motion_model_ptr_(nullptr),
    observation_model_ptr_(nullptr)
{}

template<typename MotionModelTp, typename ObservationModelTp>
KalmanFilter<MotionModelTp,
                     ObservationModelTp>::KalmanFilter(typename KalmanFilter<MotionModelTp,
                                                                                             ObservationModelTp>::motion_model_t& motion_model,
                                                               const typename KalmanFilter<MotionModelTp,
                                                                                                   ObservationModelTp>::observation_model_t& observation_model)
    :
    motion_model_ptr_(&motion_model),
    observation_model_ptr_(&observation_model)
{}

template<typename MotionModelTp, typename ObservationModelTp>
KalmanFilter<MotionModelTp,ObservationModelTp>::~KalmanFilter()
{}

template<typename MotionModelTp, typename ObservationModelTp>
void
KalmanFilter<MotionModelTp,ObservationModelTp>::set_matrix(const std::string& name, const matrix_t& mat){

    if(name != "Q" && name != "K" && name != "R" && name != "P"){
        throw std::logic_error("Invalid matrix name. Name: "+
                               name+
                               " not in [Q, K, R, P]");
    }

    matrices_.insert_or_assign(name, mat);
}

template<typename MotionModelTp, typename ObservationModelTp>
bool
KalmanFilter<MotionModelTp,ObservationModelTp>::has_matrix(const std::string& name)const{

    auto itr = matrices_.find(name);
    return itr != matrices_.end();
}

template<typename MotionModelTp, typename ObservationModelTp>
void
KalmanFilter<MotionModelTp,
                     ObservationModelTp>::estimate(const std::tuple<motion_model_input_t,
                                                   observation_model_input_t>& input ){

    predict(input.template get<0>());
    update(input.template get<1>());
}

template<typename MotionModelTp, typename ObservationModelTp>
void
KalmanFilter<MotionModelTp,ObservationModelTp>::predict(const typename KalmanFilter<MotionModelTp,
                                                                ObservationModelTp>::motion_model_input_t& u){

    if(!motion_model_ptr_){
        throw std::runtime_error("Motion model has not been set");
    }

    /// make a state predicion using the
    /// motion model
    auto& state = motion_model_ptr_->get_state();
    auto x = state.as_vector();

    /// get the matrix that describes the dynamics
    /// of the system
    auto& F = motion_model_ptr_->get_matrix("F");
    auto& B = matrices_["B"];

    /// predict the state vector
    x = F*x + B*u;
    state.set(x);

    /// predict the covariance matrix
    auto& P = matrices_["P"];
    auto& Q = matrices_["Q"];
    auto F_T = trans( F );

    P = (F*P*F_T) + Q;
}

template<typename MotionModelTp, typename ObservationModelTp>
void
KalmanFilter<MotionModelTp,
                     ObservationModelTp>::update(const typename KalmanFilter<MotionModelTp,
                                                                                     ObservationModelTp>::observation_model_input_t&  z){

    if(!motion_model_ptr_){
        throw std::runtime_error("Motion model has not been set");
    }

    if(!observation_model_ptr_){
        throw std::runtime_error("Observation model has not been set");
    }

    auto& state = motion_model_ptr_->get_state();
    auto x = state.as_vector();
    auto& P = matrices_["P"];
    auto& R = matrices_["R"];

    auto& H = observation_model_ptr_->get_matrix("H");
    auto H_T = trans(H);

    try{

      auto S = H*P*H_T + R;
      auto S_inv = inv(S);

      if(has_matrix("K")){
          auto& K = matrices_["K"];
          K = P*H_T*S_inv;
      }
      else{
          auto K = P*H_T*S_inv;
          set_matrix("K", K);
      }

      auto& K = matrices_["K"];
      auto innovation = z - H*x;

      if(K.columns() != innovation.size()){
          throw std::runtime_error("Matrix columns: "+
                                    std::to_string(K.columns())+
                                    " not equal to vector size: "+
                                    std::to_string(innovation.size()));
      }

      x += K*innovation;
      state.set(x);

      IdentityMatrix<real_t> I(state.size());

      /// update covariance matrix
      P = (I - K*H)*P;
    }
    catch(...){

        // this is a singular matrix what
        // should we do? Simply use the predicted
        // values and log the fact that there was a singular matrix

        throw;
    }
}
    
}
