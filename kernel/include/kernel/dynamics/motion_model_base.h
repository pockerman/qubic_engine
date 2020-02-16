#ifndef MOTION_MODEL_BASE_H
#define MOTION_MODEL_BASE_H

#include "boost/noncopyable.hpp"
#include <memory>
namespace kernel{
namespace dynamics{

/// \brief Base class for deriving motion models.
/// Motion models describe the dynamics or kinematics
/// of a rigid body.

template<typename StateTp, typename MatrixDescriptor, typename... InputTp>
class MotionModelBase: private boost::noncopyable
{
public:

    typedef StateTp state_t;
    typedef state_t output_t;
    typedef std::tuple<InputTp...> input_t;
    typedef MatrixDescriptor matrix_descriptor_t;
    typedef typename matrix_descriptor_t::matrix_t matrix_t;
    typedef typename matrix_descriptor_t::vector_t vector_t;

    /// \brief Destructor
    virtual ~MotionModelBase();

    /// \brief Updates and returns the value of the state given the input
    virtual state_t& evaluate(const input_t& input)=0;

    /// \brief Read access to the state
    const state_t& get_state()const{return state_;}

    /// \brief Read access to the state
    state_t& get_state(){return state_;}

    matrix_t& get_matrix(const std::string& name){return matrix_description_.get_matrix(name);}
    const matrix_t& get_matrix(const std::string& name)const{return matrix_description_.get_matrix(name);}
    void set_matrix(const std::string& name, const matrix_t& mat){matrix_description_.set_matrix(name, mat);}

    vector_t& get_vector(const std::string& name){return matrix_description_.get_vector(name);}
    const vector_t& get_vector(const std::string& name)const {return matrix_description_.get_vector(name);}
    void set_vector(const std::string& name, const vector_t& vec){matrix_description_.set_vector(name, vec);}

    /// \brief set the matrix update flag
    void set_matrix_update_flag(bool f){update_description_matrices_on_evaluate_ = f;}

    bool allows_matrix_updates()const{return update_description_matrices_on_evaluate_;}

protected:

    /// \brief Constructor
    MotionModelBase();

    /// \brief The object describing the state
    /// of the object of which its dynamics are
    /// described/modeled by this MotionModel
    state_t state_;

    /// \brief matrix descriptor
    matrix_descriptor_t matrix_description_;

    /// \brief flag indicating the update of the
    /// matrices the model is using to describe itself
    bool update_description_matrices_on_evaluate_;
};

template<typename StateTp, typename MatrixDescriptor, typename... InputTp>
MotionModelBase<StateTp, MatrixDescriptor, InputTp...>::MotionModelBase()
    :
      state_(),
      matrix_description_(),
      update_description_matrices_on_evaluate_(true)
{}

template<typename StateTp, typename MatrixDescriptor, typename... InputTp>
MotionModelBase<StateTp, MatrixDescriptor, InputTp...>::~MotionModelBase()
{}
}
}

#endif // MOTION_MODEL_BASE_H
