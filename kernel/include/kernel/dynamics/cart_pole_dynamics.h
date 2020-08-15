#ifndef CART_POLE_DYNAMICS_H
#define CART_POLE_DYNAMICS_H

#include "kernel/base/types.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/dynamics/motion_model_base.h"
#include "kernel/dynamics/dynamics_matrix_descriptor.h"

namespace kernel {
namespace dynamics {

///
/// \brief The CartPoleInput struct. Wraps the parameters
/// used as input for the CartPoleDynamics class
///
struct CartPoleInput
{
    ///
    /// \brief cart_m_. Mass of the cart
    ///
    real_t cart_m{0.0};

    ///
    /// \brief pole_m_. Mass of the pole
    ///
    real_t pole_m{0.0};

    ///
    /// \brief b_. Friction term
    ///
    real_t b{0.0};

    ///
    /// \brief f_phi_. Friction term
    ///
    real_t f_phi{0.0};

    ///
    /// \brief rod_length_. Rod length
    ///
    real_t rod_length{0.0};

    ///
    /// \brief The time step the integrator uses
    ///
    real_t dt{0.0};

    ///
    /// \brief g The gravitational constant
    ///
    real_t g;
};

///
/// \brief The CartPoleDynamics class. Describes the dynamics
/// for the Cart-Pole problem. The state of the system is described
/// by the four variables
///  \f$p\f$ the  cart position
///  \f$\dot{p}\f$ the cart velocity
///  \f$\phi\f$ the angle of the pendulum
///  \f$\dot{\phi}\f$
class CartPoleDynamics: public MotionModelDynamicsBase<SysState<4>, DynamicsMatrixDescriptor, real_t>
{

public:

    ///
    /// \brief The type of the state handled by this dynamics object
    ///
    typedef MotionModelDynamicsBase<SysState<4>,
                            DynamicsMatrixDescriptor, real_t>::state_t state_t;

    ///
    /// \brief input_t The type of the input for solving the dynamics
    ///
    typedef MotionModelDynamicsBase<SysState<4>,
                            DynamicsMatrixDescriptor, real_t>::input_t input_t;

    ///
    /// \brief matrix_t Matrix type that describes the dynamics
    ///
    typedef MotionModelDynamicsBase<SysState<4>,
                            DynamicsMatrixDescriptor, real_t>::matrix_t matrix_t;

    ///
    /// \brief vector_t
    ///
    typedef MotionModelDynamicsBase<SysState<4>,
                            DynamicsMatrixDescriptor, real_t>::vector_t vector_t;

    ///
    /// \brief CartPoleDynamics Constructor
    ///
    CartPoleDynamics();

    ///
    /// \brief CartPoleDynamics Constructor
    ///
    CartPoleDynamics(const CartPoleInput& input);

    ///
    /// \brief CartPoleDynamics Constructor
    ///
    CartPoleDynamics(const CartPoleInput& input, const DynVec<real_t>& init_state);

    ///
    /// \brief Evaluate the new state using the given input
    /// it also updates the various matrices if needed
    ///
    virtual state_t& evaluate(const input_t& input )override;

    ///
    /// \brief Integrate the new state.
    ///
    void integrate(real_t f)
    {integrate(f, 0.0);}

    ///
    /// \brief Integrate the new state. It also uses error terms
    ///
    void integrate(real_t v, real_t err);

private:


    ///
    /// \brief input_ Various constants that describe the dynamics
    ///
    CartPoleInput input_;

};

}

}

#endif // CART_POLE_DYNAMICS_H
