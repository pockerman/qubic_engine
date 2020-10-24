#ifndef EXTENDED_KALMAN_FILTER_H
#define	EXTENDED_KALMAN_FILTER_H

#include "cubic_engine/base/cubic_engine_types.h"

#include <boost/noncopyable.hpp>
#include <map>
#include <string>

namespace cengine{
namespace estimation{

///
/// Implements the Extended Kalman filter algorithm.
/// The class expects a number of inputs in order to be useful. Concretely
/// the application must specity
///
/// MotionModelTp a motion model
/// ObservationModelTp observation model
///
/// The MotionModelTp should expose the following functions
///
/// evaluate(MotionModelTp input)-->State&
/// get_matrix(const std::string)--->DynMat
///
/// In particular, the class expects the L, F matrices to be supplied via the
/// get_matix function of the motion model.
///
/// Similarly, the  ObservationModelTp should expose the following functions
///
/// evaluate(ObservationModelTp& input)--->DynVec
/// get_matrix(const std::string)--->DynMat
///
/// In particular, the class expects the M, H matrices to be supplied via the
/// get_matix function of the observation model.
///
/// Finally, the application should supply the P, Q, R matrices associated
/// with the process
///
template<typename MotionModelTp, typename ObservationModelTp>
class ExtendedKalmanFilter: private boost::noncopyable
{
public:

    typedef MotionModelTp motion_model_t;
    typedef ObservationModelTp observation_model_t;
    typedef typename motion_model_t::input_t motion_model_input_t;
    typedef typename motion_model_t::matrix_t matrix_t;
    typedef typename motion_model_t::state_t state_t;
    typedef typename observation_model_t::input_t observation_model_input_t;

    /// \brief Constructor
    ExtendedKalmanFilter();

    /// \brief Constructor
    ExtendedKalmanFilter(motion_model_t& motion_model,
                         const observation_model_t& observation_model);

    /// \brief Destructor
    ~ExtendedKalmanFilter();

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
    void predict(const motion_model_input_t& input);

    /// \brief Updates the gain matrix K, the  state vector x and covariance matrix P
    /// using the given measurement z_k according to the following equations
    ///
    /// K_k = \hat{P}_{k} * H_{k}^T * (H_k * \hat{P}_{k} * H_{k}^T + M_k * R_k * M_k^T)^{-1}
    /// x_k = \hat{x}_{k} + K_k * (z_k - h( \hat{x}_{k}, 0))
    /// P_k = (I - K_k * H_k) * \hat{P}_{k}
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
    real_t get(const std::string& name)const{return motion_model_ptr_->get_state_property(name);}

    /// \brief Returns the name-th matrix
    const DynMat<real_t>& operator[](const std::string& name)const;

    /// \brief Returns the name-th matrix
    DynMat<real_t>& operator[](const std::string& name);
           
protected:

    /// \brief pointer to the function that computes f
    motion_model_t* motion_model_ptr_;

    /// \brief pointer to the function that computes h
    const observation_model_t* observation_model_ptr_;

    /// \brief Matrices used by the filter internally
    std::map<std::string, matrix_t> matrices_;

};  

template<typename MotionModelTp, typename ObservationModelTp>
ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::ExtendedKalmanFilter()
    :
    motion_model_ptr_(nullptr),
    observation_model_ptr_(nullptr)
{}

template<typename MotionModelTp, typename ObservationModelTp>
ExtendedKalmanFilter<MotionModelTp,
                     ObservationModelTp>::ExtendedKalmanFilter(motion_model_t& motion_model,
                                                               const observation_model_t& observation_model)
    :
    motion_model_ptr_(&motion_model),
    observation_model_ptr_(&observation_model)
{}

template<typename MotionModelTp, typename ObservationModelTp>
ExtendedKalmanFilter<MotionModelTp,
                     ObservationModelTp>::~ExtendedKalmanFilter()
{}

template<typename MotionModelTp, typename ObservationModelTp>
void
ExtendedKalmanFilter<MotionModelTp,
                     ObservationModelTp>::set_matrix(const std::string& name,
                                                     const matrix_t& mat){

    if(name != "Q" && name != "K" && name != "R" && name != "P"){
        throw std::logic_error("Invalid matrix name. Name: "+
                               name+
                               " not in [Q, K, R, P]");
    }

    matrices_.insert_or_assign(name, mat);
}

template<typename MotionModelTp, typename ObservationModelTp>
bool
ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::has_matrix(const std::string& name)const{

    auto itr = matrices_.find(name);
    return itr != matrices_.end();
}

template<typename MotionModelTp, typename ObservationModelTp>
const DynMat<real_t>&
ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::operator[](const std::string& name)const{

    auto itr = matrices_.find(name);

    if(itr == matrices_.end()){
        throw std::invalid_argument("Matrix: "+name+" does not exist");
    }

    return itr->second;
}

template<typename MotionModelTp, typename ObservationModelTp>
DynMat<real_t>&
ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::operator[](const std::string& name){

    auto itr = matrices_.find(name);

    if(itr == matrices_.end()){
        throw std::invalid_argument("Matrix: "+name+" does not exist");
    }

    return itr->second;
}


template<typename MotionModelTp, typename ObservationModelTp>
void
ExtendedKalmanFilter<MotionModelTp,
                     ObservationModelTp>::estimate(const std::tuple<motion_model_input_t,
                                                   observation_model_input_t>& input ){

    predict(input.template get<0>());
    update(input.template get<1>());
}

template<typename MotionModelTp, typename ObservationModelTp>
void
ExtendedKalmanFilter<MotionModelTp,ObservationModelTp>::predict(const motion_model_input_t& u){

    /// make a state predicion using the
    /// motion model
    auto& state = motion_model_ptr_->evaluate(u);

    auto& P = (*this)["P"];
    auto& Q = (*this)["Q"];

    auto& L = motion_model_ptr_->get_matrix("L");
    auto L_T = trans(L);

    auto& F = motion_model_ptr_->get_matrix("F");
    auto F_T = trans(F);

    P = F * P * F_T + L*Q*L_T;
}

template<typename MotionModelTp, typename ObservationModelTp>
void
ExtendedKalmanFilter<MotionModelTp,
                     ObservationModelTp>::update(const observation_model_input_t&  z){

    auto& state = motion_model_ptr_->get_state();
    auto& P = (*this)["P"];
    auto& R = (*this)["R"];

    auto zpred = observation_model_ptr_->evaluate(z);

    auto& H = observation_model_ptr_->get_matrix("H");
    auto H_T = trans(H);

    // compute \partial{h}/\partial{v} the jacobian of the observation model
    // w.r.t the error vector
    auto& M = observation_model_ptr_->get_matrix("M");
    auto M_T = trans(M);

     try{

        // S = H*P*H^T + M*R*M^T
        auto S = H*P*H_T + M*R*M_T;

        auto S_inv = inv(S);

        if(has_matrix("K")){
            auto& K = (*this)["K"];
            K = P*H_T*S_inv;
        }
        else{
            auto K = P*H_T*S_inv;
            set_matrix("K", K);
        }

        auto& K = (*this)["K"];

        auto innovation = z - zpred;

        if(K.columns() != innovation.size()){
            throw std::runtime_error("Matrix columns: "+
                                      std::to_string(K.columns())+
                                      " not equal to vector size: "+
                                      std::to_string(innovation.size()));
        }

        state.add(K*innovation);

        IdentityMatrix<real_t> I(state.size());

        /// update the covariance matrix
        P =  (I - K*H)*P;
    }
    catch(...){

        // this is a singular matrix what
        // should we do? Simply use the predicted
        // values and log the fact that there was a singular matrix

        throw;
    }
}


}
}
#endif	/* EXTENDED_KALMAN_FILTER_H */

