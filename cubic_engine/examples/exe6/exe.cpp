#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/base/kernel_consts.h"
#include "cubic_engine/rl/worlds/grid_world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "cubic_engine/rl/q_learning.h"
#include "cubic_engine/rl/reward_table.h"


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
using cengine::rl::QTableLearning;
using cengine::rl::QLearningInput;
using cengine::rl::RewardTable;

const uint_t N_CELLS = 7;
const real_t PENALTY = -1000;



class RewardProducer
{
public:

    typedef real_t value_t;

    /// construcotr
    RewardProducer();

    /// returns the reward for the goal
    real_t goal_reward()const{return 8.0;}

    /// returns the reward for the action
    /// at  state s when going to state sprime
    template<typename ActionTp, typename StateTp>
    real_t get_reward(const ActionTp& action,
                      const StateTp& s,
                      const StateTp& sprime)const;

private:

    /// table that holds the rewards
    RewardTable<GridWorldAction, real_t> rewards_;

    /// setup the rewards
    void setup_rewards();
};

RewardProducer::RewardProducer()
    :
      rewards_()
{
    setup_rewards();
}

void
RewardProducer::setup_rewards(){

    for(uint_t i=0; i<N_CELLS*N_CELLS; ++i){

        if(i<7){

            if(i != 6){
                rewards_.add_reward(i, GridWorldAction::EAST,  PENALTY);
            }

            rewards_.add_reward(i, GridWorldAction::NORTH,  PENALTY);

            if(i!= 0){
                rewards_.add_reward(i, GridWorldAction::WEST,  PENALTY);
            }
        }
        else if(i>=42){

        }
        else{
            static const uint_t ar[]={7,14,21,28,35,13,20,27,34};

        }

    }
}

template<typename ActionTp, typename StateTp>
real_t
RewardProducer::get_reward(const ActionTp& action,
                           const StateTp& s,
                           const StateTp& sprime)const{

    return rewards_.get_reward(s.get_id(), action);

}

class RewardProducer;
typedef GridWorld<RewardProducer> world_t;
typedef world_t::state_t state_t;

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
       if(i <7){

           state.set_transition(static_cast<GridWorldAction>(GridWorldAction::SOUTH), nullptr);

           if(i != 6){
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
       else if(i >= 42 ){
           /// top row

           state.set_transition(static_cast<GridWorldAction>(GridWorldAction::SOUTH), &w.get_state(i - N_CELLS));

           if(i != 48){
             state.set_transition(GridWorldAction::EAST, &w.get_state(i+1));
           }
           else{
               state.set_transition(GridWorldAction::EAST, nullptr);
           }

           state.set_transition(GridWorldAction::NORTH, nullptr);

           if(i == 42){
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), nullptr);
           }
           else{
              state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &w.get_state(i-1));
           }
       }
       else{

           /// all rows in between
           state.set_transition(static_cast<GridWorldAction>(GridWorldAction::SOUTH), &w.get_state(i - N_CELLS));

           if(i != 13 && i != 20 && i != 27 && i != 41){
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), &w.get_state(i +1));
           }
           else{
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), nullptr);
           }

           state.set_transition(static_cast<GridWorldAction>(GridWorldAction::NORTH), &w.get_state(i + N_CELLS));

           if(i != 7 && i != 14 && i != 21 && i != 28 && i != 35){
              state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &w.get_state(i-1));
           }
           else {
              state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), nullptr);
           }
       }
   }
}

}

int main() {

    using namespace exe;

    /// the world of the agent
    world_t world;

    create_wolrd(world);

    state_t start(10);
    state_t goal(12);

    /// simulation parameters
    const real_t EPSILON = 0.7;

    QLearningInput qinput={0.01, EPSILON, 1.0, true};

    QTableLearning<world_t> qlearner(std::move(qinput));


    /// number of episodes for the agent to
    /// learn the Q-values.
    const uint_t N_ITERATIONS = 10000;

    for(uint_t episode=0; episode < N_ITERATIONS; ++episode){
        world.restart(start, goal);
        qlearner.train(world);
    }
    
   return 0;
}

