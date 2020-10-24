#ifndef MOTION_MODEL_BASE_H
#define MOTION_MODEL_BASE_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"

#include "boost/noncopyable.hpp"
#include <memory>
#include <vector>
#include <string>

namespace kernel{
namespace dynamics{

/// \brief Base class for deriving motion models.
/// Motion models describe the dynamics or kinematics
/// of a rigid body.
template<typename StateTp, typename MatrixDescriptor, typename InputTp>
class MotionModelDynamicsBase: private boost::noncopyable
{
public:

    typedef StateTp state_t;
    typedef state_t output_t;
    typedef InputTp input_t;
    typedef MatrixDescriptor matrix_descriptor_t;
    typedef typename matrix_descriptor_t::matrix_t matrix_t;
    typedef typename matrix_descriptor_t::vector_t vector_t;

    ///
    /// \brief The dimension of the state
    ///
    static const uint_t state_dimension = StateTp::dimension;

    ///
    /// \brief Destructor
    ///
    virtual ~MotionModelDynamicsBase();

    ///
    /// \brief Updates and returns
    /// the value of the state given the input
    ///
    virtual state_t& evaluate(const input_t& input)=0;

    ///
    /// \brief Read access to the state
    ///
    const state_t& get_state()const{return state_;}

    ///
    /// \brief Read/write access to the state
    ///
    state_t& get_state(){return state_;}

    ///
    /// \brief get_state_variables_names. Returns the name of the
    /// variables in the state
    ///
    std::vector<std::string_view> get_state_variables_names()const
    {return state_.get_names();}

    matrix_t& get_matrix(const std::string& name){return matrix_description_.get_matrix(name);}
    const matrix_t& get_matrix(const std::string& name)const{return matrix_description_.get_matrix(name);}
    void set_matrix(const std::string& name, const matrix_t& mat){matrix_description_.set_matrix(name, mat);}

    vector_t& get_vector(const std::string& name){return matrix_description_.get_vector(name);}
    const vector_t& get_vector(const std::string& name)const {return matrix_description_.get_vector(name);}
    void set_vector(const std::string& name, const vector_t& vec){matrix_description_.set_vector(name, vec);}

    ///
    /// \brief set the matrix update flag
    ///
    void set_matrix_update_flag(bool f){update_description_matrices_on_evaluate_ = f;}

    ///
    /// \brief Set the flag for updating or not the matrices describing
    /// the model
    ///
    bool allows_matrix_updates()const{return update_description_matrices_on_evaluate_;}

    ///
    /// \brief Returns true if the matrix with the given name 
    /// already exists
    ///
    bool has_matrix(const std::string& name)const
    {return matrix_description_.has_matrix(name);}

    ///
    /// \brief Returns the state property with the given name
    ///
    real_t get_state_property(const std::string& name)const{return state_.get(name);}

    ///
    /// \brief Set the name-th value of the state
    ///
    void set_state_property(const std::string& name, real_t value)
    {state_.set(name,value);}

    /// \brief Set the state names
    void set_state_name_value(uint_t i, const std::string& name, real_t val)
    {state_.set(i, {name, val});}

    ///
    /// \brief Set the state names
    ///
    void set_state_name_value(uint_t i, const std::pair<std::string, real_t>&  val)
    {state_.set(i, val);}


    ///
    /// \brief Set the time step
    ///
    void set_time_step(real_t dt){dt_ = dt;}

    ///
    /// \brief get_time_step Returns the sampling time the
    /// dynamics model is using
    ///
    real_t get_time_step()const{return dt_;}

    ///
    /// \brief get_tolerance Returns the general
    /// tolerance used in the calculations
    ///
    real_t get_tolerance()const{return tol_;}

    ///
    /// \brief set_tolerance Set the general tolerance used
    /// in the calculations. Default is KernelConsts::tolerance()
    ///
    void set_tolerance(real_t tol)const{tol_ = tol;}

protected:

    /// \brief Constructor
    MotionModelDynamicsBase();

    ///
    /// \brief The object describing the state
    /// of the object of which its dynamics are
    /// described/modeled by this MotionModel
    ///
    state_t state_;

    ///
    /// \brief matrix descriptor
    ///
    matrix_descriptor_t matrix_description_;

    ///
    /// \brief flag indicating the update of the
    /// matrices the model is using to describe itself
    ///
    bool update_description_matrices_on_evaluate_;

    ///
    /// \brief The time step the integrator uses
    ///
    real_t dt_;

    ///
    /// \brief tolerance
    ///
    real_t tol_;

};

template<typename StateTp, typename MatrixDescriptor, typename InputTp>
MotionModelDynamicsBase<StateTp, MatrixDescriptor, InputTp>::MotionModelDynamicsBase()
    :
      state_(),
      matrix_description_(),
      update_description_matrices_on_evaluate_(true),
      dt_(0.0),
      tol_(KernelConsts::tolerance())
{}

template<typename StateTp, typename MatrixDescriptor, typename InputTp>
MotionModelDynamicsBase<StateTp, MatrixDescriptor, InputTp>::~MotionModelDynamicsBase()
{}
}
}

#endif // MOTION_MODEL_BASE_H
