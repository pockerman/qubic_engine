#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/control/mpc_control.h"
#include "cubic_engine/estimation/kalman_filter.h"

#include "kernel/base/angle_calculator.h"
#include "kernel/base/physics_constants.h"
#include "kernel/base/unit_converter.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/dynamics/cart_pole_dynamics.h"
#include "kernel/maths/constants.h"
#include "kernel/maths/direct_solvers/blaze_direct_solver.h"
#include "kernel/maths/optimization/admm.h"
#include "kernel/maths/matrix_utilities.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/utilities/common_uitls.h"



#include <cmath>
#include <iostream>
#include <tuple>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::DynVec;
using cengine::control::MPCConfig;
using cengine::control::MPCController;
using cengine::estimation::KalmanFilter;
using kernel::dynamics::SysState;
using kernel::dynamics::CartPoleConfig;
using kernel::dynamics::CartPoleDynamics;
using kernel::maths::opt::ADMMConfig;
using kernel::maths::opt::ADMM;
using kernel::Null;


// Problem constants
const uint_t N_STEPS = 300;
const real_t DT = 0.001;
const real_t M = 0.5;
const real_t m = 0.2;
const real_t L = 0.3;
const real_t b = 0.1;
const real_t fphi = 0.1;
const real_t G = kernel::PhysicsConsts::gravity_constant();
const real_t phi0 = 15*2*kernel::MathConsts::PI/360.;

class Observer
{
public:

    typedef Null config_t;

    Observer(const config_t&)
    {}

};
class ObservationModel
{

public:

    typedef Null input_t;

    const DynMat<real_t>& get_matrix(const std::string& name)const{}

};


typedef KalmanFilter<CartPoleDynamics, ObservationModel> kalman_filter_t;

}

int main() {
   
    using namespace example;

    // wrap the system constants
    CartPoleConfig cpconfig = {M, m, b, fphi, L, DT, G};

    // initial state
    DynVec<real_t> init_state={0, 0, phi0, 0};

    try{

        // cart-pole dynamics instance
        CartPoleDynamics dynamics(cpconfig, init_state);

        // observation model
        ObservationModel obs_model;

        typedef MPCConfig<ADMM<DynMat<real_t>, DynVec<real_t>>,
                          Observer, kalman_filter_t> mpc_config_t;

        typedef MPCController<ADMM<DynMat<real_t>, DynVec<real_t>>,
                Observer, kalman_filter_t> mpc_control_t;

        typedef typename mpc_control_t::input_t mpc_input_t;
        mpc_input_t mpc_input;

        // configuration of the controller
        mpc_config_t   config;

        std::cout<<"Set up configuration for Quadratic problem"<<std::endl;

        // reference state
        config.x_ref = DynVec<real_t>({0.3, 0.0, 0.0, 0.0});

        std::cout<<"Set up configuration for Kalman Filter"<<std::endl;

        // set up configuration for Kalman Filter
        config.estimator_config.Q = 10. * kernel::create_identity_matrix<real_t>(init_state.size());

        // set up configuration for Kalman Filter
        config.estimator_config.R = kernel::create_identity_matrix<real_t>(2);

        // set up configuration for Kalman Filter
        config.estimator_config.P = kernel::create_identity_matrix<real_t>(init_state.size());

        // set up configuration for Kalman Filter
        config.estimator_config.B = kernel::create_identity_matrix<real_t>(init_state.size());

        config.estimator_config.motion_model = &dynamics;
        config.estimator_config.observation_model = &obs_model;


        // MPC controller
        mpc_control_t mpc_control(config);

        std::cout<<"Setup MPC quadratic problem"<<std::endl;

        // minimum constraints
        mpc_control.get_qp().l =  DynVec<real_t>({-1.0, -100, -100, -100});

        // maximum constraints
        mpc_control.get_qp().u = DynVec<real_t>({1.0, 100.0, 100, 100});

        // setup cost for states
        mpc_control.get_qp().P = kernel::create_diagonal_matrix<real_t>({1.0, 0, 5.0, 0});


        std::cout<<"Starting simulation"<<std::endl;

        // loop over the MPC steps
        for(uint_t s=0; s<N_STEPS; ++s){

            mpc_control.solve(mpc_input);

        }

    }
    catch(std::runtime_error& e){
        std::cerr<<"Runtime error: "
                 <<e.what()<<std::endl;
    }
    catch(std::logic_error& e){
        std::cerr<<"Logic error: "
                 <<e.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }
   
    return 0;
}

