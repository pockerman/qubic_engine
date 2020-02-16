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

class DiffDriveDynamics: public MotionModelBase<SysState<3>, DynamicsMatrixDescriptor, real_t, real_t, std::array<real_t, 2>>
{
public:

    /// \brief Tge type of the state handled by this dynamics object
    typedef MotionModelBase<SysState<3>, DynamicsMatrixDescriptor, real_t, real_t, std::array<real_t, 2> >::state_t state_t;
    typedef MotionModelBase<SysState<3>, DynamicsMatrixDescriptor, real_t, real_t, std::array<real_t, 2> >::input_t input_t;

    /// \brief Constructor
    DiffDriveDynamics();

    /// \brief Constructor
    DiffDriveDynamics(state_t&& state);

    /// \brief Evaluate the new state using the given input
    /// it also updates the various matrices if needed
    virtual state_t& evaluate(const input_t& input )override;

    /// \brief Integrate the new state. The input is
    /// the distance Dk and orientation
    void integrate(real_t distance, real_t orientation){integrate(distance, orientation, {{0.0, 0.0}});}

    /// \brief Integrate the new state. The input is
    /// the distance Dk and orientation. It also uses error terms
    void integrate(real_t distance, real_t orientation, const std::array<real_t, 2>& errors);

    /// \brief Read the x-coordinate
    real_t get_x_position()const{return this->state_.get("X");}

    /// \brief Read the y-coordinate
    real_t get_y_position()const{return this->state_.get("Y");}

    /// \brief Read the y-coordinate
    real_t get_orientation()const{return this->state_.get("Theta");}

    /// \brief updates the matrices used to describe this
    /// motion model
    void update_matrices(const input_t& input);

protected:



};

}

}

#endif // DIFF_DRIVE_DYNAMICS_H
