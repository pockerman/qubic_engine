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
        world.create_world();

        std::cout<<"Number of states: "<<world.n_states()<<std::endl;

        state_t start(0);
        state_t goal(11);

        /// simulation parameters
        /// number of episodes for the agent to learn.
        const uint_t N_ITERATIONS = 500;
        const real_t ETA = 0.1;
        const real_t EPSILON = 0.3;
        const real_t GAMMA = 0.0;
        const real_t PENALTY = -100.0;

        SarsaLearningInput qinput={ETA, EPSILON, GAMMA, true, true};
        SarsaTableLearning<world_t> sarsalearner(std::move(qinput));

        CSVWriter writer("agent_rewards.csv", ',', true);
        writer.write_column_names({"Episode", "Reward"}, true);

        std::vector<real_t> row(2);
        sarsalearner.initialize(world, PENALTY);

        auto& table = sarsalearner.get_table();
        table.save_to_csv("table_rewards" + std::to_string(0) + ".csv");

        for(uint_t episode=0; episode < N_ITERATIONS; ++episode){

            std::cout<<"At episode: "<<episode<<std::endl;
            world.restart(start, goal);
            auto result = sarsalearner.train(goal);

            /// the total reward the agent obtained
            /// in this episode
            auto reward = result.total_reward;
            writer.write_row(std::make_tuple(episode, reward));
            std::cout<<"At episode: "<<episode<<" total reward: "<<reward<<std::endl;

            if(episode == N_ITERATIONS - 1){
                auto& table = sarsalearner.get_table();
                table.save_to_csv("table_rewards" + std::to_string(episode) + ".csv");
            }
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
