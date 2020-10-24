#include "cubic_engine/rl/worlds/cliff_world.h"
#include <algorithm>
#include <iostream>
namespace cengine{
namespace rl{
namespace worlds{

CliffWorld::CliffWorld()
    :
      World<GridWorldAction, GridWorldState,  real_t>(),
      start_(nullptr),
      goal_(nullptr),
      current_state_(nullptr),
      reward_(),
      r_(0.0),
      states_(),
      finished_(false)
{}

CliffWorld::~CliffWorld()
{}


void
CliffWorld::step(const CliffWorld::action_t& action){

    if(states_.empty()){
        throw std::logic_error("Cell connectivity is not established");
    }

    if(current_state_ == nullptr ){
       throw std::logic_error("Null current state pointer");
    }

    if(goal_ == nullptr){
       throw std::logic_error("Null goal pointer");
    }

    if(*current_state_ == *goal_){
        r_ = 0.0;
        finished_ = true;
    }
    else{

        /// for the current state
        /// find out the index of the cell
        /// that the agent can transition to
        auto* next_state = const_cast<state_t*>(current_state_)->execute_action(action);

        if(next_state == nullptr){
            // the agent just came out of the grid
            // so finishe the game
            finished_ = true;
        }
        else{
            r_ = reward_.get_reward(action, *current_state_, *next_state);
            current_state_ = next_state;

            static const uint_t cliff[]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

            if(std::find(&cliff[0],
                         &cliff[ sizeof(cliff)/sizeof(uint_t) ], current_state_->get_id()) !=
                    &cliff[ sizeof (cliff)/sizeof(uint_t) ]){

                finished_ = true;
            }
        }
    }
}

void
CliffWorld::create_world(){

    const uint_t N_CELLS = 48;
    const uint_t N_CELLS_X = 12;
    states_.reserve(N_CELLS);

    /// create default states
    for(uint_t state=0; state<N_CELLS; ++state){
       states_.push_back(GridWorldState(state));
    }

    for(auto& state:states_){

        auto id = state.get_id();

        if(id <12){

            state.set_transition(static_cast<GridWorldAction>(GridWorldAction::SOUTH), &state);

            if(id==0){
              state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &state);
            }
            else{
                state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &states_[id-1]);
            }

            if(id == 11){
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), nullptr);
            }
            else{
              state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), &states_[id + 1]);
            }

            state.set_transition(static_cast<GridWorldAction>(GridWorldAction::NORTH), &states_[id + N_CELLS_X]);

        }
        else if (id>=36) {
            state.set_transition(static_cast<GridWorldAction>(GridWorldAction::NORTH), &state);
            state.set_transition(static_cast<GridWorldAction>(GridWorldAction::SOUTH), &states_[id - N_CELLS_X]);

            if(id == 36){
                state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &state);
            }
            else{
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &states_[id - 1]);
            }

            if(id == 47){
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), &state);
            }
            else{
                state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), &states_[id + 1]);
            }
        }
        else{

            state.set_transition(static_cast<GridWorldAction>(GridWorldAction::NORTH), &states_[id + N_CELLS_X]);
            state.set_transition(static_cast<GridWorldAction>(GridWorldAction::SOUTH), &states_[id - N_CELLS_X]);

            if(id == 12 || id == 24){
                state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &state);
            }
            else{
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &states_[id - 1]);
            }

            if(id == 23 || id == 35){
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), &state);
            }
            else{
                state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), &states_[id + 1]);
            }
        }
    }

    /// initialize the rewards
    reward_.setup_rewards();
}

 CliffWorld::RewardProducer::RewardProducer()
     :
       rewards_()
 {}

 real_t
 CliffWorld::RewardProducer::get_reward(const action_t& action,
                                        const state_t& s,
                                        const state_t& sprime)const{
     return rewards_.get_reward(s.get_id(), action);
 }

void
CliffWorld::RewardProducer::setup_rewards(){

    const uint_t N_CELLS_X = 12;
    const uint_t N_CELLS_Y = 4;
    const real_t PENALTY = -100.0;
    const real_t NORMAL = -1.0;

    for(uint_t i=0; i<N_CELLS_X*N_CELLS_Y; ++i){

        if(i<11){

            if(i==0){
               rewards_.add_reward(i, GridWorldAction::EAST,  PENALTY);
               rewards_.add_reward(i, GridWorldAction::NORTH,  NORMAL);
               rewards_.add_reward(i, GridWorldAction::SOUTH,  NORMAL);
               rewards_.add_reward(i, GridWorldAction::WEST,  NORMAL);
            }
            else if( i == 11){
                rewards_.add_reward(i, GridWorldAction::WEST,  PENALTY);
                rewards_.add_reward(i, GridWorldAction::NORTH,  NORMAL);
                rewards_.add_reward(i, GridWorldAction::SOUTH,  NORMAL);
                rewards_.add_reward(i, GridWorldAction::EAST,  NORMAL);
             }
        }
        else {

            if(i >= 36 ){

                rewards_.add_reward(i, GridWorldAction::NORTH,  NORMAL);
                rewards_.add_reward(i, GridWorldAction::SOUTH,  NORMAL);

                if(i != 47){
                  rewards_.add_reward(i, GridWorldAction::EAST,  NORMAL);
                }
                else{
                    rewards_.add_reward(i, GridWorldAction::EAST,  NORMAL);
                }

                if(i != 36){
                   rewards_.add_reward(i, GridWorldAction::WEST,  NORMAL);
                }
                else{
                    rewards_.add_reward(i, GridWorldAction::WEST,  NORMAL);
                }
            }
            else{

                rewards_.add_reward(i, GridWorldAction::NORTH,  NORMAL);

                if(i != 24 || i != 12){
                   rewards_.add_reward(i, GridWorldAction::WEST,  NORMAL);
                }
                else{
                    rewards_.add_reward(i, GridWorldAction::WEST,  NORMAL);
                }

                if(i != 23 || i != 35){
                  rewards_.add_reward(i, GridWorldAction::EAST,  NORMAL);
                }
                else{
                   rewards_.add_reward(i, GridWorldAction::EAST,  NORMAL);
                }

                if( i >= 24){
                    rewards_.add_reward(i, GridWorldAction::SOUTH,  NORMAL);
                }

               if( i>=13 && i<=22){
                   rewards_.add_reward(i, GridWorldAction::SOUTH,  PENALTY);
               }

               if(i == 12){
                 rewards_.add_reward(i, GridWorldAction::SOUTH,  NORMAL);
               }

               if(i == 23){
                  rewards_.add_reward(i, GridWorldAction::SOUTH,  0.0);
               }
            }
        }

    }/// for

}

}
}
}
