#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/rl/worlds/cliff_world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"

#include <algorithm>
#include <iostream>

namespace cengine{
namespace rl{
namespace worlds{

CliffWorld::CliffWorld()
    :
      DiscreteWorld<GridWorldAction, GridWorldState,
                    cliff_world_detail::CliffWorldRewardProducer,
                    ConstantEnvironmentDynamics>(),
      r_(0.0)
{}


const CliffWorld::action_t
CliffWorld::sample_action()const{
    return uniform_sample_grid_world_action();
}


std::tuple<CliffWorld::state_t*, real_t, bool, std::any>
CliffWorld::step(const CliffWorld::action_t& action){

    if(this->states_.empty()){
        throw std::logic_error("Cell connectivity is not established");
    }

    if(this->current_state_ == nullptr ){
       throw std::logic_error("Null current state pointer");
    }

    if(this->goals_.empty()){
        throw std::logic_error("Goal is not established");
    }

    if(this->goals_[0] == nullptr){
       throw std::logic_error("Null goal pointer");
    }

    auto* goal = this->goals_[0];

    if(*this->current_state_ == *goal){
        r_ = 0.0;
        this->finished_ = true;
        return {const_cast<state_t*>(this->current_state_), r_, true, std::any()};
    }
    else{

        /// for the current state
        /// find out the index of the cell
        /// that the agent can transition to
        auto* next_state = const_cast<state_t*>(this->current_state_)->execute_action(action);

        if(next_state == nullptr){
            // the agent just came out of the grid
            // so finishe the game
            this->finished_ = true;
        }
        else{
            r_ = this->reward_.get_reward(action, *this->current_state_, *next_state);
            this->current_state_ = next_state;

            static const uint_t cliff[]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

            if(std::find(&cliff[0],
                         &cliff[ sizeof(cliff)/sizeof(uint_t) ], this->current_state_->get_id()) !=
                    &cliff[ sizeof (cliff)/sizeof(uint_t) ]){

                this->finished_ = true;
            }
        }

        return {next_state, r_, this->finished_ , std::any()};
    }
}

void
CliffWorld::create_world(){

    const uint_t N_CELLS = 48;
    const uint_t N_CELLS_X = 12;
    this->states_.reserve(N_CELLS);

    /// create default states
    for(uint_t state=0; state<N_CELLS; ++state){
       this->states_.push_back(GridWorldState(state));
    }

    for(auto& state:this->states_){

        auto id = state.get_id();

        if(id <12){

            state.set_transition(static_cast<GridWorldAction>(GridWorldAction::SOUTH), &state);

            if(id==0){
              state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &state);
            }
            else{
                state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &this->states_[id-1]);
            }

            if(id == 11){
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), nullptr);
            }
            else{
              state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), &this->states_[id + 1]);
            }

            state.set_transition(static_cast<GridWorldAction>(GridWorldAction::NORTH), &this->states_[id + N_CELLS_X]);

        }
        else if (id>=36) {
            state.set_transition(static_cast<GridWorldAction>(GridWorldAction::NORTH), &state);
            state.set_transition(static_cast<GridWorldAction>(GridWorldAction::SOUTH), &this->states_[id - N_CELLS_X]);

            if(id == 36){
                state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &state);
            }
            else{
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &this->states_[id - 1]);
            }

            if(id == 47){
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), &state);
            }
            else{
                state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), &this->states_[id + 1]);
            }
        }
        else{

            state.set_transition(static_cast<GridWorldAction>(GridWorldAction::NORTH), &this->states_[id + N_CELLS_X]);
            state.set_transition(static_cast<GridWorldAction>(GridWorldAction::SOUTH), &this->states_[id - N_CELLS_X]);

            if(id == 12 || id == 24){
                state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &state);
            }
            else{
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::WEST), &this->states_[id - 1]);
            }

            if(id == 23 || id == 35){
               state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), &state);
            }
            else{
                state.set_transition(static_cast<GridWorldAction>(GridWorldAction::EAST), &this->states_[id + 1]);
            }
        }
    }

    /// initialize the rewards
    reward_.setup_rewards();
}

namespace cliff_world_detail
{
     CliffWorldRewardProducer::CliffWorldRewardProducer()
         :
           rewards_()
     {}

    real_t
    CliffWorldRewardProducer::get_reward(const CliffWorldRewardProducer::action_t& action,
                                         const CliffWorldRewardProducer::state_t& s,
                                         const CliffWorldRewardProducer::state_t& sprime)const{
         return rewards_.get_reward(s.get_id(), action);
     }

    void
    CliffWorldRewardProducer::setup_rewards(){

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
}// cliff_world_detail

}
}
}

#endif
