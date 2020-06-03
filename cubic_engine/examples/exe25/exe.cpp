#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/csv_file_writer.h"
#include "cubic_engine/rl/worlds/grid_world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "cubic_engine/rl/synchronous_value_function_learning.h"
#include "cubic_engine/rl/reward_table.h"

#include <cmath>
#include <utility>
#include <tuple>
#include <iostream>
#include <random>
#include <algorithm>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::rl::worlds::GridWorld;
using cengine::rl::worlds::GridWorldAction;
using cengine::rl::SyncValueFuncItr;
using cengine::rl::SyncValueFuncItrInput;
using cengine::rl::RewardTable;
using kernel::CSVWriter;

class RewardProducer
{
public:

    typedef real_t value_t;

    /// construcotr
    RewardProducer();

    /// returns the reward for the goal
    real_t goal_reward()const{return 0.0;}

    /// returns the reward for the action
    /// at  state s when going to state sprime
    template<typename ActionTp, typename StateTp>
    real_t get_reward(const ActionTp& action,
                      const StateTp& s,
                      const StateTp& sprime)const{
        return rewards_.get_reward(s.get_id(), action);
    }

    // returns the reward for the action
        /// at  state s when going to state sprime
        template<typename ActionTp, typename StateTp>
        real_t get_reward(const ActionTp& action,
                          const StateTp& s)const{
            return rewards_.get_reward(s.get_id(), action);
        }

private:

    /// table that holds the rewards
    RewardTable<GridWorldAction, real_t> rewards_;

    /// setup the rewards
    void setup_rewards();
};

RewardProducer::RewardProducer()
    :
   rewards_()
{}

void
RewardProducer::setup_rewards(){

    rewards_.set_reward(0, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(0, GridWorldAction::NORTH, -1.0);

    rewards_.set_reward(1, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(1, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(1, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(2, GridWorldAction::EAST, 0.0);
    rewards_.set_reward(2, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(2, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(4, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(4, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(4, GridWorldAction::SOUTH, -1.0);

    rewards_.set_reward(5, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(5, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(5, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(5, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(6, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(6, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(6, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(6, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(7, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(7, GridWorldAction::SOUTH, 0.0);
    rewards_.set_reward(7, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(8, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(8, GridWorldAction::NORTH, 0.0);
    rewards_.set_reward(8, GridWorldAction::SOUTH, -1.0);

    rewards_.set_reward(9, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(9, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(9, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(9, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(10, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(10, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(10, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(10, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(11, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(11, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(11, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(13, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(13, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(13, GridWorldAction::WEST, 0.0);

    rewards_.set_reward(14, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(14, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(14, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(15, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(15, GridWorldAction::WEST, -1.0);
}

typedef GridWorld<RewardProducer> world_t;
typedef world_t::state_t state_t;

const uint_t N_CELLS = 4;

void
create_wolrd(world_t& w){

   std::vector<state_t> world_states;
   world_states.reserve(N_CELLS*N_CELLS);

   uint_t counter=0;
   for(uint_t i=0; i<N_CELLS; ++i){
       for(uint_t j=0; j<N_CELLS; ++j){
           world_states.push_back(state_t(counter++));
       }
   }

   w.set_states(std::move(world_states));

   counter=0;
   for(uint_t i=0; i<N_CELLS*N_CELLS; ++i){

       auto& state = w.get_state(i);

       /// bottom row
       if(i <4){

           state.set_transition(static_cast<GridWorldAction>(GridWorldAction::SOUTH), nullptr);

           if(i != 3){
             state.set_transition(GridWorldAction::EAST, &w.get_state(i+1));
           }
           else{
               state.set_transition(GridWorldAction::EAST, nullptr);
           }

           state.set_transition(GridWorldAction::NORTH, &w.get_state(N_CELLS + i));

           if(i == 0){
                state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), nullptr);
           }
           else{
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &w.get_state(i-1));
           }
       }
       else if(i >= 12 ){
           /// top row

           state.set_transition(static_cast<GridWorldAction>(GridWorldAction::SOUTH), &w.get_state(i - N_CELLS));

           if(i != 15){
             state.set_transition(GridWorldAction::EAST, &w.get_state(i+1));
           }
           else{
               state.set_transition(GridWorldAction::EAST, nullptr);
           }

           state.set_transition(GridWorldAction::NORTH, nullptr);

           if(i == 12){
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), nullptr);
           }
           else{
              state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &w.get_state(i-1));
           }
       }
       else{

           /// all rows in between
           state.set_transition(static_cast<GridWorldAction>(GridWorldAction::SOUTH), &w.get_state(i - N_CELLS));

           if(i != 11 && i != 7){
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), &w.get_state(i +1));
           }
           else{
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), nullptr);
           }

           state.set_transition(static_cast<GridWorldAction>(GridWorldAction::NORTH), &w.get_state(i + N_CELLS));

           if(i != 4 && i != 8 ){
              state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &w.get_state(i-1));
           }
           else {
              state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), nullptr);
           }
       }
   }
}

}

int main(){

    using namespace example;

    try{

        typedef GridWorld<RewardProducer> world_t;
        typedef world_t::state_t state_t;
        typedef world_t::action_t action_t;

        auto policy = [](const action_t&,const state_t&){
          return 0.25;
        };

        auto dynamics = [](const state_t&, real_t, const state_t&, const action_t&){
          return 1.0;
        };

        std::vector<real_t> rewards(1, -1.0);

        /// the world of the agent
        world_t world;
        create_wolrd(world);


        std::cout<<"Number of states: "<<world.n_states()<<std::endl;

        state_t start(0);
        state_t goal1(3);
        state_t goal2(12);

        world.append_goal(goal1);
        world.append_goal(goal2);

        /// simulation parameters
        /// number of episodes for the agent to learn.
        const uint_t N_ITERATIONS = 500;
        const real_t TOL = 1.0e-8;
        const real_t EPSILON = 0.3;
        const real_t GAMMA = 1.0;
        const real_t PENALTY = -100.0;

        SyncValueFuncItrInput input={TOL, GAMMA, N_ITERATIONS, true};
        SyncValueFuncItr<world_t> learner(std::move(input));

        //CSVWriter writer("agent_rewards.csv", ',', true);
        //writer.write_column_names({"Episode", "Reward"}, true);

        std::vector<real_t> row(2);
        learner.initialize(world, PENALTY);

        world.restart(start);
        auto result = learner.train(policy, dynamics, rewards);

        //auto& table = sarsalearner.get_table();
        //table.save_to_csv("table_rewards" + std::to_string(0) + ".csv");

        /*for(uint_t episode=0; episode < N_ITERATIONS; ++episode){

            std::cout<<"At episode: "<<episode<<std::endl;
            world.restart(start, goal);
            auto result = learner.train();

            /// the total reward the agent obtained
            /// in this episode
            auto reward = result.total_reward;
            writer.write_row(std::make_tuple(episode, reward));
            std::cout<<"At episode: "<<episode<<" total reward: "<<reward<<std::endl;

            if(episode == N_ITERATIONS - 1){
                auto& table = sarsalearner.get_table();
                table.save_to_csv("table_rewards" + std::to_string(episode) + ".csv");
            }
        }*/
    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
