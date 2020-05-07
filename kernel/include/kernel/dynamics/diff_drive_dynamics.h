#ifndef DIFF_DRIVE_DYNAMICS_H
#define DIFF_DRIVE_DYNAMICS_H

#include "kernel/base/types.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/dynamics/motion_model_base.h"
#include "kernel/dynamics/dynamics_matrix_descriptor.h"

#include <array>
namespace kernel{
namespace dynamics{

/// \brief DiffDriveDynamics class. Describes the
/// motion dynamics of a differential drive system. It implements
/// the following equations
class DiffDriveDynamics: public MotionModelBase<SysState<3>, DynamicsMatrixDescriptor,
                                                real_t, real_t, std::array<real_t, 2>>
{
public:

    /// \brief Tge type of the state handled by this dynamics object
    typedef MotionModelBase<SysState<3>,
                            DynamicsMatrixDescriptor,
                            real_t, real_t, std::array<real_t, 2> >::state_t state_t;
    typedef MotionModelBase<SysState<3>,
                            DynamicsMatrixDescriptor,
                            real_t, real_t, std::array<real_t, 2> >::input_t input_t;

    typedef MotionModelBase<SysState<3>,
                            DynamicsMatrixDescriptor,
                            real_t, real_t, std::array<real_t, 2> >::matrix_t matrix_t;

    typedef MotionModelBase<SysState<3>,
                            DynamicsMatrixDescriptor,
                            real_t, real_t, std::array<real_t, 2> >::vector_t vector_t;

    /// \brief Constructor
    DiffDriveDynamics();

    /// \brief Constructor
    DiffDriveDynamics(state_t&& state);

    /// \brief Evaluate the new state using the given input
    /// it also updates the various matrices if needed
    virtual state_t& evaluate(const input_t& input )override;

    /// \brief Integrate the new state.
    void integrate(real_t v, real_t w)
    {integrate(v, w, {{0.0, 0.0}});}

    /// \brief Integrate the new state. It also uses error terms
    void integrate(real_t v, real_t w, const std::array<real_t, 2>& errors);

    /// \brief Read the x-coordinate
    real_t get_x_position()const{return this->state_.get("X");}

    /// \brief Set the x-coordinate
    real_t set_x_position(real_t x){this->state_.set("X", x);}

    /// \brief Read the y-coordinate
    real_t get_y_position()const{return this->state_.get("Y");}

    /// \brief Set the y-coordinate
    real_t set_y_position(real_t y){this->state_.set("Y", y);}

    /// \brief Read the y-coordinate
    real_t get_orientation()const{return this->state_.get("Theta");}

    /// \brief Set the orientation
    void set_orientation(real_t theta){this->state_.set("Theta", theta);}

    /// \brief updates the matrices used to describe this
    /// motion model
    void update_matrices(const input_t& input);

    /// \brief Set the time step
    void set_time_step(real_t dt){dt_ = dt;}

    /// \brief Set the tolerance to use
    real_t set_tolerance(real_t tol){tol_ = tol;}

    /// \brief Initialize the matrices describing the
    /// the dynamics
    void initialize_matrices(const input_t& input);

private:

    /// \brief The time step the integrator uses
    real_t dt_;

    /// \brief tolerance
    real_t tol_;

};

}

}

#endif // DIFF_DRIVE_DYNAMICS_H
