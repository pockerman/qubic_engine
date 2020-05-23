#ifndef UNSCENTED_KALMAN_FILTER_H
#define UNSCENTED_KALMAN_FILTER_H

#include "cubic_engine/base/cubic_engine_types.h"

#include <boost/noncopyable.hpp>
#include <map>
#include <vector>
#include <string>

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

};

template<typename MotionModelTp, typename ObservationModelTp>
UnscentedKalmanFilter<MotionModelTp,ObservationModelTp>::UnscentedKalmanFilter()
    :
    motion_model_ptr_(nullptr),
    observation_model_ptr_(nullptr),
    matrices_(),
    sigma_points_(),
    w_()
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
    w_()
{}

template<typename MotionModelTp,
         typename ObservationModelTp>
UnscentedKalmanFilter<MotionModelTp,
                      ObservationModelTp>::~UnscentedKalmanFilter()
{}

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

}

}

#endif // UNSCENTED_KALMAN_FILTER_H
