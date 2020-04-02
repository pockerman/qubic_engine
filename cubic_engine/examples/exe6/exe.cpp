#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/csv_file_writer.h"
#include "cubic_engine/rl/worlds/grid_world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"


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
using cengine::rl::worlds::GridWorld;
using cengine::rl::worlds::GridWorldAction;

class RewardProducer
{

};



}

int main(int argc, char** argv) {

    using namespace exe;

    typedef GridWorld<GridWorldAction, RewardProducer>::state_t state_t;

    /// the world of the agent
    GridWorld<GridWorldAction, RewardProducer> world(7, 7);

    state_t start = 10;
    state_t goal = 12;

    /// simulation parameters
    const real_t EPSILON = 0.7;

    /// number of episodes for the agent to
    /// learn the Q-values.
    const uint_t N_ITERATIONS = 10000;

    for(uint_t episode=0; episode < N_ITERATIONS; ++episode){
        world.restart(start, goal);
    }






    
   return 0;
}

