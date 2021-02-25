#ifndef GRID_WORLD_H
#define GRID_WORLD_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "cubic_engine/rl/worlds/grid_world_state.h"
#include "cubic_engine/rl/discrete_world.h"
#include "kernel/base/kernel_consts.h"

#include <map>
#include <vector>
#include <stdexcept>

namespace cengine {
namespace rl {
namespace worlds {

///
/// \brief GridWorld class. Models the grid world environment
///
template<typename RewardTp>
class GridWorld final: public DiscreteWorld<GridWorldAction,
                                            GridWorldState,
                                            RewardTp>
{
public:

    typedef typename DiscreteWorld<GridWorldAction, GridWorldState, RewardTp>::reward_t reward_t;
    typedef typename DiscreteWorld<GridWorldAction, GridWorldState, RewardTp>::action_t action_t;
    typedef typename DiscreteWorld<GridWorldAction, GridWorldState, RewardTp>::state_t state_t;
    typedef typename DiscreteWorld<GridWorldAction, GridWorldState, RewardTp>::reward_value_t reward_value_t;

    ///
    /// \brief Global invalid action assumed by the
    /// world. Useful for initialization.
    ///
    static const GridWorldAction invalid_action = GridWorldAction::INVALID_ACTION;

    ///
    /// \brief Constructor
    ///
    GridWorld();

    ///
    /// \brief Destructor
    ///
    ~GridWorld();

    ///
    /// \brief Returns the state
    ///
    virtual const state_t& sense()override final{return  *this->current_state_;}

    ///
    /// \brief Transition to a new state by
    /// performing the given action
    ///
    virtual void step(const action_t&)override final;

    ///
    /// \brief Returns the reward associated
    /// with the last state transition
    ///
    reward_value_t reward()const{return r_;}

    ///
    /// \brief Execute the aid-th action in the current state
    ///
    void execute_action(action_t aid);

    ///
    /// \brief build The world with nx elements in the x-direction
    /// and ny-elements in the y-direction
    ///
    void build(const uint_t nx, const uint_t ny);

private:


    ///
    /// \brief The reward that the agent should recieve
    /// after executing an action
    ///
    reward_value_t r_;
};

template<typename RewardTp>
GridWorld<RewardTp>::GridWorld()
    :
    DiscreteWorld<GridWorldAction, GridWorldState, RewardTp>(),
    r_(0.0)
{}

template<typename RewardTp>
GridWorld<RewardTp>::~GridWorld()
{}

template<typename RewardTp>
void
GridWorld<RewardTp>::step(const typename GridWorld<RewardTp>::action_t& action){

    if(this->states_.empty()){
        throw std::logic_error("Cell connectivity is not established");
    }

    if(this->current_state_ == nullptr ){
       throw std::logic_error("Null current state pointer");
    }

    if(this->goals_.empty()){
       throw std::logic_error("Empty goals list");
    }

    if(this->is_goal_state(*this->current_state_)){
        r_ = this->reward_.goal_reward();
        this->finished_ = true;
    }
    else{

        // for the current state
        // find out the index of the cell
        // that the agent can transition to
        auto* next_state = const_cast<state_t*>(this->current_state_)->execute_action(action);

        if(next_state == nullptr){

            // the agent just came out of the grid
            // so finish the game
            this->finished_ = true;

            // what is the return for this?
            r_ = this->reward_.get_reward(action, *this->current_state_);
        }
        else{
            r_ = this->reward_.get_reward(action, *this->current_state_, *next_state);
            this->current_state_ = next_state;
        }
    }
}


template<typename RewardTp>
void
GridWorld<RewardTp>::execute_action(typename GridWorld<RewardTp>::action_t aid){

    this->current_state_ = const_cast<state_t*>(this->current_state_)->execute_action(aid);
}


template<typename RewardTp>
void
GridWorld<RewardTp>::build(const uint_t nx, const uint_t ny){

    // clear what ever state we may have
    this->states_.clear();
    this->states_.reserve(nx*ny);
    std::vector<std::string> cell_type(nx*ny);

    uint_t counter=0;

    for(uint_t j=0; j<ny; ++j){
        for(uint_t i=0; i<nx; ++i){
            this->states_.push_back(state_t(counter));

            // bottom row
            if( j == 0){

                if(i == 0){
                    cell_type[counter] = "BOTTOM_LEFT_CORNER";
                }
                else if( i != nx -1 ){
                    cell_type[counter] = "BOTTOM";
                }
                else{
                   cell_type[counter] = "BOTTOM_RIGHT_CORNER";
                }
            }
            else if( j != ny - 1){

                // middle cells
                if( i % nx == 0){
                    cell_type[counter] = "MOST_LEFT_COLUMN";
                }
                else if( i != nx -1 ){

                    cell_type[counter] = "MIDDLE";
                }
                else{
                   cell_type[counter] = "MOST_RIGHT_COLUMN";
                }
            }
            else{

                // top row cells
                if( i % nx == 0){
                    cell_type[counter] = "TOP_LEFT_CORNER";
                }
                else if( i != nx -1 ){

                    cell_type[counter] = "TOP";
                }
                else{
                   cell_type[counter] = "TOP_RIGHT_CORNER";
                }
            }

            counter++;
        }
    }

    // we now buid the state-action association
    counter = 0;
    for(uint_t i=0; i<nx*ny; ++i){

            auto& state = this->get_state(i);
            auto c_type = cell_type[i];

            if(c_type == "BOTTOM_LEFT_CORNER"){


                // fall back to itself for SOUTH and WEST
                state.set_transition(GridWorldAction::SOUTH, &state);
                state.set_transition(GridWorldAction::EAST, &this->get_state(i + 1));
                state.set_transition(GridWorldAction::NORTH, &this->get_state(nx + i));
                state.set_transition(GridWorldAction::WEST, &state);
            }
            else if(c_type == "BOTTOM"){

                // fall back to itself for SOUTH
                state.set_transition(GridWorldAction::SOUTH, &state);
                state.set_transition(GridWorldAction::EAST, &this->get_state(i + 1));
                state.set_transition(GridWorldAction::NORTH, &this->get_state(nx + i));
                state.set_transition(GridWorldAction::WEST, &this->get_state(i - 1));
            }
            else if(c_type == "BOTTOM_RIGHT_CORNER"){

                // fall back to itself for SOUTH and EAST
                state.set_transition(GridWorldAction::SOUTH, &state);
                state.set_transition(GridWorldAction::EAST, &state);
                state.set_transition(GridWorldAction::NORTH, &this->get_state(nx + i));
                state.set_transition(GridWorldAction::WEST, &this->get_state(i - 1));
            }
            else if(c_type == "MOST_LEFT_COLUMN"){

                // fall back to itself for SOUTH and EAST
                state.set_transition(GridWorldAction::SOUTH, &this->get_state(nx - i));
                state.set_transition(GridWorldAction::EAST, &this->get_state(i + 1));
                state.set_transition(GridWorldAction::NORTH, &this->get_state(nx + i));
                state.set_transition(GridWorldAction::WEST, &state);
            }
            else if(c_type == "MIDDLE"){

                state.set_transition(GridWorldAction::SOUTH, &this->get_state(nx - i));
                state.set_transition(GridWorldAction::EAST, &this->get_state(i + 1));
                state.set_transition(GridWorldAction::NORTH, &this->get_state(nx + i));
                state.set_transition(GridWorldAction::WEST, &this->get_state(i - 1));
            }
            else if(c_type == "MOST_RIGHT_COLUMN"){

                state.set_transition(GridWorldAction::SOUTH, &this->get_state(nx - i));
                state.set_transition(GridWorldAction::EAST, &state);
                state.set_transition(GridWorldAction::NORTH, &this->get_state(nx + i));
                state.set_transition(GridWorldAction::WEST, &this->get_state(i - 1));

            }
            else if(c_type == "TOP_LEFT_CORNER"){

                state.set_transition(GridWorldAction::SOUTH, &this->get_state(nx - i));
                state.set_transition(GridWorldAction::EAST, &this->get_state(i + 1));
                state.set_transition(GridWorldAction::NORTH, &state);
                state.set_transition(GridWorldAction::WEST, &state);
            }
            else if(c_type == "TOP"){

                state.set_transition(GridWorldAction::SOUTH, &this->get_state(nx - i));
                state.set_transition(GridWorldAction::EAST, &this->get_state(i + 1));
                state.set_transition(GridWorldAction::NORTH, &state);
                state.set_transition(GridWorldAction::WEST, &this->get_state(i - 1));
            }
            else if(c_type == "TOP_RIGHT_CORNER"){

                state.set_transition(GridWorldAction::SOUTH, &this->get_state(nx - i));
                state.set_transition(GridWorldAction::EAST, &state);
                state.set_transition(GridWorldAction::NORTH, &state);
                state.set_transition(GridWorldAction::WEST, &this->get_state(i - 1));
            }
            else{
                throw std::logic_error("Cell type " + c_type +" is unknown");
            }
        }
}

}
}
}
#endif
#endif // GRID_WORLD_H
