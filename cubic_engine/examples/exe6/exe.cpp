#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/csv_file_writer.h"
#include "cubic_engine/rl/worlds/cliff_world.h"


#include <cmath>
#include <utility>
#include <tuple>
#include <iostream>
#include <random>
#include <algorithm>

namespace exe
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::rl::worlds::CliffWorld;



}

int main(int argc, char** argv) {

   using namespace exe;

    /// the world of the agent
    CliffWorld world(30, 15);

    /// simulation parameters
    const real_t EPSILON = 0.7;

    /// number of episodes for the agent to
    /// learn the Q-values.
    const uint_t N_ITERATIONS = 10000;

    for(uint_t episode=0; episode < N_ITERATIONS; ++episode){
        world.restart();
    }






    
   return 0;
}

