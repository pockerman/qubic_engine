#include <nlopt.hpp> 

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/control/mpc_control.h"
#include "cubic_engine/estimation/kalman_filter.h"
#include "kernel/dynamics/cart_pole_dynamics.h"
#include "kernel/base/angle_calculator.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/maths/constants.h"
#include "kernel/base/unit_converter.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/base/physics_constants.h"

#include <cmath>
#include <iostream>
#include <tuple>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::DynVec;
using cengine::control::MPCInput;
using cengine::control::MPCController;
using cengine::estimation::KalmanFilter;
using kernel::dynamics::SysState;
using kernel::dynamics::CartPoleInput;
using kernel::dynamics::CartPoleDynamics;

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




}

int main() {
   
    using namespace example;

    // wrap the system constants
    CartPoleInput cpin = {M, m, b, fphi, L, DT, G};

    // initial state
    DynVec<real_t> init_state={0, 0, phi0, 0};

    try{

        // cart-pole dynamics instance
        CartPoleDynamics dynmics(cpin, init_state);

        // input to the controller
        MPCInput input;

        // loop over the MPC steps
        for(uint_t s=0; s<N_STEPS; ++s){

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

