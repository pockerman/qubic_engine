#ifndef BICYCLE_VEHICLE_MODEL_DYNAMICS_H
#define BICYCLE_VEHICLE_MODEL_DYNAMICS_H

#include "kernel/base/types.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/dynamics/motion_model_base.h"
#include "kernel/dynamics/dynamics_matrix_descriptor.h"

namespace kernel{
namespace dynamics{

///
/// \brief The BicycleVehicleModelDynamics class.
/// Implements the bicycle vehicle model dynamics.
///
class BicycleVehicleModelDynamics: public MotionModelDynamicsBase<SysState<10>, DynamicsMatrixDescriptor,
                                                                  real_t, real_t, std::array<real_t, 2>>
{

public:
    typedef MotionModelDynamicsBase<SysState<10>, DynamicsMatrixDescriptor,
                                    real_t, real_t, std::array<real_t, 2>> base_t;
    typedef base_t::state_t state_t;
    typedef state_t output_t;
    typedef base_t::input_t input_t;
    typedef base_t::matrix_descriptor_t matrix_descriptor_t;
    typedef typename matrix_descriptor_t::matrix_t matrix_t;
    typedef typename matrix_descriptor_t::vector_t vector_t;

    ///
    /// \brief Constructor
    ///
    BicycleVehicleModelDynamics();

    ///
    /// \brief integrate.
    ///
    void integrate(const input_t& input );

    ///
    /// \brief evaluate
    /// \param input
    /// \return
    ///
    state_t& evaluate(const input_t& input );

    ///
    /// \brief initialize_matrices. Initialize the matrix description of the
    /// dynamics
    ///
    void initialize_matrices(const input_t& input);

    ///
    /// \brief update_matrices. Update the matrix description of the
    /// dynamics
    ///
    void update_matrices(const input_t& input);

private:

};

}
}

#endif // BICYCLE_VEHICLE_MODEL_DYNAMICS_H
