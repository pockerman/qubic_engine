#ifndef CART_POLE_DYNAMICS_H
#define CART_POLE_DYNAMICS_H

#include "kernel/base/types.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/dynamics/motion_model_base.h"
#include "kernel/dynamics/dynamics_matrix_descriptor.h"

#include "boost/any.hpp"
#include <string>

namespace kernel {
namespace dynamics {

///
/// \brief The CartPoleInput struct. Wraps the parameters
/// used as input for the CartPoleDynamics class
///
struct CartPoleConfig
{
    ///
    /// \brief cart_m_. Mass of the cart
    ///
    real_t M{0.0};

    ///
    /// \brief pole_m_. Mass of the pole
    ///
    real_t m{0.0};

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
class CartPoleDynamics: public MotionModelDynamicsBase<SysState<4>,
        DynamicsMatrixDescriptor, std::map<std::string, boost::any>>
{

public:

    ///
    /// \brief The type of the state handled by this dynamics object
    ///
    typedef MotionModelDynamicsBase<SysState<4>,
                            DynamicsMatrixDescriptor, std::map<std::string, boost::any>>::state_t state_t;

    ///
    /// \brief input_t The type of the input for solving the dynamics
    ///
    typedef MotionModelDynamicsBase<SysState<4>,
                            DynamicsMatrixDescriptor, std::map<std::string, boost::any>>::input_t input_t;

    ///
    /// \brief matrix_t Matrix type that describes the dynamics
    ///
    typedef MotionModelDynamicsBase<SysState<4>,
                            DynamicsMatrixDescriptor, std::map<std::string, boost::any>>::matrix_t matrix_t;

    ///
    /// \brief vector_t
    ///
    typedef MotionModelDynamicsBase<SysState<4>,
                            DynamicsMatrixDescriptor, std::map<std::string, boost::any>>::vector_t vector_t;

    ///
    /// \brief CartPoleDynamics Constructor
    ///
    CartPoleDynamics();

    ///
    /// \brief CartPoleDynamics Constructor
    ///
    CartPoleDynamics(const CartPoleConfig& input);

    ///
    /// \brief CartPoleDynamics Constructor
    ///
    CartPoleDynamics(const CartPoleConfig& input, const DynVec<real_t>& init_state);

    ///
    /// \brief Evaluate the new state using the given input
    /// it also updates the various matrices if needed
    ///
    virtual state_t& evaluate(const input_t& input )override;

    ///
    /// \brief Integrate the new state.
    ///
    void integrate(const input_t& input);

    ///
    /// \brief updates the matrices used to describe this
    /// motion model
    ///
    void update_matrices(const input_t& input);

    ///
    /// \brief Initialize the matrices describing the
    /// the dynamics
    ///
    void initialize_matrices(const input_t& input);

    ///
    /// \brief load_from_json Load the dynamics configuration  from
    /// the given json file
    ///
    void load_from_json(const std::string& filename){};


private:


    ///
    /// \brief input_ Various constants that describe the dynamics
    ///
    CartPoleConfig config_;

};

}

}

#endif // CART_POLE_DYNAMICS_H
