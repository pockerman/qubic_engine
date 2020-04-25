#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/csv_file_writer.h"
#include "cubic_engine/rl/worlds/cliff_world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "cubic_engine/rl/tabular_sarsa_learning.h"
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
    using cengine::rl::SarsaTableLearning;
    using cengine::rl::SarsaLearningInput;
    using cengine::rl::RewardTable;
    using kernel::CSVWriter;

    try{

        typedef CliffWorld world_t;
        typedef world_t::state_t state_t;

        /// the world of the agent
        CliffWorld world;

        std::cout<<"Number of states: "<<world.n_states()<<std::endl;

        state_t start(36);
        state_t goal(12);

        /// simulation parameters
        const real_t EPSILON = 0.1;
        const real_t PENALTY = -100.0;

        SarsaLearningInput qinput={1.0, EPSILON, 0.0, true, true};
        SarsaTableLearning<world_t> sarsalearner(std::move(qinput));

        CSVWriter writer("agent_rewards.csv", ',', true);
        writer.write_column_names({"Episode", "Reward"}, true);

        /// number of episodes for the agent to
        /// learn the Q-values.
        const uint_t N_ITERATIONS = 10000;

        std::vector<real_t> row(2);
        sarsalearner.initialize(world, PENALTY);

        for(uint_t episode=0; episode < N_ITERATIONS; ++episode){
            world.restart(start, goal);
            sarsalearner.train(goal);

            auto reward = sarsalearner.get_table().get_total_reward();
            writer.write_row(std::make_tuple(episode, reward));
            std::cout<<"At episode: "<<episode<<" total reward: "<<reward<<std::endl;
        }

        /// now that we train let's play
        auto& qtable = sarsalearner.get_table();

        auto stop = false;

        world.restart(start, goal);

        auto counter = 0;
        while(!stop){

            auto& state = world.get_current_state();
            std::cout<<"At state: "<<state.get_id()<<std::endl;

            if(state == goal){
                std::cout<<"State: "<<state.get_id()<<" is the goal"<<std::endl;
                break;
            }

            /// get the action with the maximum value
            auto action = qtable.get_max_reward_action_at_state(state.get_id());

            std::cout<<"Maximum reward action: "<<cengine::rl::worlds::to_string(action)<<std::endl;
            std::cout<<"MAximum reward is: "<<qtable.get_max_reward_at_state(state.get_id())<<std::endl;
            world.execute_action(action);
            counter++;
        }
    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
