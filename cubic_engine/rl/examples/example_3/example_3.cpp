#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/csv_file_writer.h"
#include "cubic_engine/rl/worlds/cliff_world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "cubic_engine/rl/sarsa_learning.h"
#include "cubic_engine/rl/reward_table.h"

#include <cmath>
#include <utility>
#include <tuple>
#include <iostream>
#include <random>
#include <algorithm>

int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::rl::worlds::CliffWorld;
    using cengine::rl::worlds::GridWorldAction;
    using cengine::rl::Sarsa;
    using cengine::rl::TDInput;
    using cengine::rl::RewardTable;
    using kernel::utilities::CSVWriter;

    try{

        typedef CliffWorld world_t;
        typedef world_t::state_t state_t;

        /// the world of the agent
        CliffWorld world;
        world.create_world();

        std::cout<<"Number of states: "<<world.n_states()<<std::endl;

        state_t start(0);
        state_t goal(11);

        /// simulation parameters
        /// number of episodes for the agent to learn.
        const uint_t N_ITERATIONS = 500;
        const real_t ETA = 0.1;
        const real_t EPSILON = 0.1;
        const real_t GAMMA = 1.0;
        const real_t PENALTY = -100.0;

        TDInput tdinput;
        tdinput.learning_rate =ETA;
        tdinput.epsilon = EPSILON;
        tdinput.discount_factor = GAMMA;
        tdinput.show_iterations = true;
        tdinput.max_num_iterations = 100;
        tdinput.total_episodes = 1000;
        tdinput.random_seed = 42;

        Sarsa<world_t> sarsalearner(tdinput);
        sarsalearner.initialize(world, PENALTY);
        sarsalearner.train();

    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}

#endif
