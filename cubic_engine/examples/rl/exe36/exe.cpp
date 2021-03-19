#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/constant_environment_dynamics.h"
#include "cubic_engine/rl/policy_evaluator.h"
#include "cubic_engine/rl/policy_improvement.h"
#include "cubic_engine/rl/worlds/grid_world.h"

#include "kernel/utilities/csv_file_writer.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/array_utils.h"
#include "kernel/maths/matrix_utilities.h"

#include <cmath>
#include <utility>
#include <tuple>
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include <limits>
#include <map>

namespace example
{
using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::rl::worlds::GridWorld;
using cengine::rl::PolicyEvaluator;
using cengine::rl::ConstantEnvironmentDynamics;
using cengine::rl::PolicyIteration;
}

int main(){


    using namespace example;


    try{

    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
#else
#include <iostream>
int main(){
    std::cerr<<"This example requires RL support. Configure CubicEngine library with RL support"<<std::endl;
    return 0;
}
#endif
