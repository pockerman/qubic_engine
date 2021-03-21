#ifndef QUADROTOR_DYNAMICS_H
#define QUADROTOR_DYNAMICS_H

#include "kernel/base/types.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/dynamics/motion_model_base.h"
#include "kernel/dynamics/dynamics_matrix_descriptor.h"

#include "boost/any.hpp"

namespace kernel {
namespace dynamics {

///
/// \brief The QuadrotorDynamics class. Implements quadrotor dynamics
///
class QuadrotorDynamics: public MotionModelDynamicsBase<SysState<12>,
        DynamicsMatrixDescriptor, std::map<std::string, boost::any>>
{
public:

    ///
    /// \brief The type of the state handled by this dynamics object
    ///
    typedef MotionModelDynamicsBase<SysState<12>,
                            DynamicsMatrixDescriptor, std::map<std::string, boost::any>>::state_t state_t;

    ///
    /// \brief input_t The type of the input for solving the dynamics
    ///
    typedef MotionModelDynamicsBase<SysState<12>,
                            DynamicsMatrixDescriptor,
                            std::map<std::string, boost::any>>::input_t input_t;

    ///
    /// \brief matrix_t Matrix type that describes the dynamics
    ///
    typedef MotionModelDynamicsBase<SysState<12>,
                            DynamicsMatrixDescriptor,
                            std::map<std::string, boost::any>>::matrix_t matrix_t;

    ///
    /// \brief vector_t
    ///
    typedef MotionModelDynamicsBase<SysState<12>,
                            DynamicsMatrixDescriptor,
                            std::map<std::string, boost::any>>::vector_t vector_t;

    ///
    /// \brief CartPoleDynamics Constructor
    ///
    QuadrotorDynamics();

    ///
    /// \brief Evaluate the new state using the given input
    /// it also updates the various matrices if needed
    ///
    virtual state_t& evaluate(const input_t& input )override;

    ///
    /// \brief Integrate the new state. It also uses error terms
    ///
    void integrate(const input_t& input);


};

}

}

#endif // QUADROTOR_DYNAMICS_H
