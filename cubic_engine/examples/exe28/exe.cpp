#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/control/mpc_control.h"
#include "cubic_engine/estimation/kalman_filter.h"

#include "kernel/base/angle_calculator.h"
#include "kernel/base/physics_constants.h"
#include "kernel/base/unit_converter.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/dynamics/cart_pole_dynamics.h"
#include "kernel/maths/constants.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/maths/direct_solvers/blaze_direct_solver.h"
#include "kernel/maths/optimization/admm.h"


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
        CartPoleDynamics dynmics(cpconfig, init_state);

        typedef MPCConfig<ADMM<DynMat<real_t>, DynVec<real_t>>,
                          Observer, kalman_filter_t> mpc_config_t;

        typedef MPCController<ADMM<DynMat<real_t>, DynVec<real_t>>,
                Observer, kalman_filter_t> mpc_control_t;

        typedef typename mpc_control_t::input_t mpc_input_t;
        mpc_input_t mpc_input;

        // configuration of the controller
        mpc_config_t   config;

        config.min = DynVec<real_t>({-1.0, -100, -100, -100});
        config.max = DynVec<real_t>({1.0, 100.0, 100, 100});
        config.x_ref = DynVec<real_t>({0.3, 0.0, 0.0, 0.0});

        // MPC controller
        mpc_control_t mpc_control(config);

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

