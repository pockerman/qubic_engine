#ifndef DIFF_DRIVE_DYNAMICS_H
#define DIFF_DRIVE_DYNAMICS_H

#include "kernel/base/types.h"
#include "kernel/dynamics/system_state.h"

#include <array>
namespace kernel
{
namespace dynamics
{


/// \brief DiffDriveDynamics class. Describes the
/// motion dynamics of a differential drive system. It implements
/// the following equations

class DiffDriveDynamics
{
public:

    typedef SysState<3> state_type;

    /// \brief Constructor
    DiffDriveDynamics(state_type&& state);

    /// \brief Integrate the new state. The input is
    /// the distance Dk and orientation
    void integrate(real_t distance, real_t orientation){integrate(distance, orientation, {{0.0, 0.0}});}

    /// \brief Integrate the new state. The input is
    /// the distance Dk and orientation. It also uses error terms
    void integrate(real_t distance, real_t orientation, const std::array<real_t, 2>& errors);

private:

    /// \brief Object describing the state of the dynamics
    state_type state_;
};

}

}

#endif // DIFF_DRIVE_DYNAMICS_H
