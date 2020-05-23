#ifndef UNSCENTED_KALMAN_FILTER_H
#define UNSCENTED_KALMAN_FILTER_H

#include "cubic_engine/base/cubic_engine_types.h"

#include <boost/noncopyable.hpp>
#include <map>
#include <vector>
#include <string>
#include <stdexcept>

namespace cengine {
namespace estimation {

template<typename MotionModelTp, typename ObservationModelTp>
class UnscentedKalmanFilter: private boost::noncopyable
{
public:

    typedef MotionModelTp motion_model_t;
    typedef ObservationModelTp observation_model_t;
    typedef typename motion_model_t::input_t motion_model_input_t;
    typedef typename motion_model_t::matrix_t matrix_t;
    typedef typename motion_model_t::state_t state_t;
    typedef typename observation_model_t::input_t observation_model_input_t;

    /// \brief Constructor
    UnscentedKalmanFilter();

    /// \brief Constructor
    UnscentedKalmanFilter(motion_model_t& motion_model,
                         const observation_model_t& observation_model);

    /// \brief Destructor
    ~UnscentedKalmanFilter();

    /// \brief Estimate the state. This function simply
    /// wraps the predict and update steps described by the
    /// functions below
    void estimate(const std::tuple<motion_model_input_t, observation_model_input_t>& input );

    /// \brief Predicts the state vector x and the process covariance matrix P using
    /// the given input control u accroding to the following equations
    void predict(const motion_model_input_t& input);

    /// \brief Updates the gain matrix K, the  state vector x and covariance matrix P
    /// using the given measurement z_k according to the following equations
    void update(const observation_model_input_t& z);

    /// \brief Set the motion model
    void set_motion_model(motion_model_t& motion_model)
    {motion_model_ptr_ = &motion_model;}

    /// \brief Set the observation model
    void set_observation_model(const observation_model_t& observation_model)
    {observation_model_ptr_ = &observation_model;}

    /// \brief Set the matrix used by the filter
    void set_matrix(const std::string& name, const matrix_t& mat);

    /// \brief Set the k parameter
    void set_k(real_t k){k_=k;}

    /// \brief Returns true if the matrix with the given name exists
    bool has_matrix(const std::string& name)const;

    /// \brief Returns the state
    const state_t& get_state()const{return motion_model_ptr_->get_state();}

    /// \brief Returns the state
    state_t& get_state(){return motion_model_ptr_->get_state();}

    /// \brief Returns the state property with the given name
    real_t get(const std::string& name)const{return motion_model_ptr_->get(name);}

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

    /// \brief An array that holds the sigma points
    std::vector<DynVec<real_t>> sigma_points_;

    /// \brief An array that holds the weights
    /// corresponding to each sigma point
    std::vector<real_t> w_;

    /// \brief The k parameter of the model
    real_t k_;

    /// \brief Check the state of the instance before
    /// doing any computations
    void check_sanity_()const;

};

template<typename MotionModelTp, typename ObservationModelTp>
UnscentedKalmanFilter<MotionModelTp,ObservationModelTp>::UnscentedKalmanFilter()
    :
    motion_model_ptr_(nullptr),
    observation_model_ptr_(nullptr),
    matrices_(),
    sigma_points_(),
    w_(),
    k_(0.0)
{}

template<typename MotionModelTp, typename ObservationModelTp>
UnscentedKalmanFilter<MotionModelTp,
                     ObservationModelTp>::UnscentedKalmanFilter(motion_model_t& motion_model,
                                                                const observation_model_t& observation_model)
    :
    motion_model_ptr_(&motion_model),
    observation_model_ptr_(&observation_model),
    matrices_(),
    sigma_points_(),
    w_(),
    k_(0.0)
{}

template<typename MotionModelTp,
         typename ObservationModelTp>
UnscentedKalmanFilter<MotionModelTp,
                      ObservationModelTp>::~UnscentedKalmanFilter()
{}

template<typename MotionModelTp,
         typename ObservationModelTp>
void
UnscentedKalmanFilter<MotionModelTp,
ObservationModelTp>::check_sanity_()const{

    if(!motion_model_ptr_){
      throw std::logic_error("Motion model has not been set");
    }

    if(!observation_model_ptr_){
      throw std::logic_error("Observation model has not been set");
    }

    if(sigma_points_.empty()){
        throw std::logic_error("Sigma points list is empty");
    }

    if(w_.empty()){
        throw std::logic_error("Weights list is empty");
    }

    if(w_.size() != sigma_points_.size()){
        throw std::logic_error("Weights and sigma poinst lists have incompatible sizes");
    }

}

template<typename MotionModelTp, typename ObservationModelTp>
void
UnscentedKalmanFilter<MotionModelTp,
                      ObservationModelTp>::set_matrix(const std::string& name, const matrix_t& mat){

    if(name != "Q" && name != "K" && name != "R" && name != "P"){
        throw std::logic_error("Invalid matrix name. Name: "+
                               name+
                               " not in [Q, K, R, P]");
    }

    matrices_.insert_or_assign(name, mat);
}

template<typename MotionModelTp, typename ObservationModelTp>
bool
UnscentedKalmanFilter<MotionModelTp,
                      ObservationModelTp>::has_matrix(const std::string& name)const{

    auto itr = matrices_.find(name);
    return itr != matrices_.end();
}

template<typename MotionModelTp, typename ObservationModelTp>
const DynMat<real_t>&
UnscentedKalmanFilter<MotionModelTp,
                      ObservationModelTp>::operator[](const std::string& name)const{

    auto itr = matrices_.find(name);

    if(itr == matrices_.end()){
        throw std::invalid_argument("Matrix: "+name+" does not exist");
    }

    return itr->second;
}

template<typename MotionModelTp, typename ObservationModelTp>
DynMat<real_t>&
UnscentedKalmanFilter<MotionModelTp,
                      ObservationModelTp>::operator[](const std::string& name){

    auto itr = matrices_.find(name);

    if(itr == matrices_.end()){
        throw std::invalid_argument("Matrix: "+name+" does not exist");
    }

    return itr->second;
}

template<typename MotionModelTp, typename ObservationModelTp>
void
UnscentedKalmanFilter<MotionModelTp,
                     ObservationModelTp>::estimate(const std::tuple<motion_model_input_t,
                                                    observation_model_input_t>& input ){

    predict(input.template get<0>());
    update(input.template get<1>());
}

template<typename MotionModelTp, typename ObservationModelTp>
void
UnscentedKalmanFilter<MotionModelTp,
                      ObservationModelTp>::predict(const motion_model_input_t& u){


    check_sanity_();

    static auto point_propagator = [this, &u](DynVec<real_t>& point){
        point = motion_model_ptr_->evaluate(point, u);
    };

    /// propagate the sigma points
    std::for_each(sigma_points_.begin(),
                  sigma_points_.end(),
                  point_propagator);

    DynVec<real_t> total_state(get_state().dimension, 0.0);

    /// update the state
    for(uint_t p=0; p<sigma_points_.size(); ++p){
        total_state += w_[p]*sigma_points_[p];
    }

    /// state = sum w_i*p_i
    get_state().set(total_state);

    DynMat<real_t> total_cov(get_state().dimension,
                             get_state().dimension, 0.0);

    /// predict the covariance matrix
    /// update the state
    for(uint_t p=0; p<sigma_points_.size(); ++p){
        total_cov += w_[p]*(sigma_points_[p] - total_state)*trans(sigma_points_[p] - total_state);
    }

    auto& P = (*this)["P"];
    auto& Q = (*this)["Q"];

    P = total_cov + Q;
}

template<typename MotionModelTp, typename ObservationModelTp>
void
UnscentedKalmanFilter<MotionModelTp,
                     ObservationModelTp>::update(const observation_model_input_t&  z){

    check_sanity_();

    /*auto& state = motion_model_ptr_->get_state();
    auto& P = (*this)["P"];
    auto& R = (*this)["R"];

    auto zpred = observation_model_ptr_->evaluate(z);

    auto& H = observation_model_ptr_->get_matrix("H");
    auto H_T = trans(H);

    /// compute \partial{h}/\partial{v} the jacobian of the observation model
    /// w.r.t the error vector
    auto& M = observation_model_ptr_->get_matrix("M");
    auto M_T = trans(M);

     try{

        /// S = H*P*H^T + M*R*M^T
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
    }*/
}

}
}

#endif // UNSCENTED_KALMAN_FILTER_H
