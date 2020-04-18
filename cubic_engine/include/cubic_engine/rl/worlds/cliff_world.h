#ifndef CLIFF_WORLD_H
#define CLIFF_WORLD_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"

namespace cengine{
namespace rl{
namespace worlds {

// The phase space

// This defines external world. Here, we have the following descrete
// world. '#' is a cliff. When the agent goes there, is restarts at
// state S and is given a -CLIFF_REWARD reward. Otherwise, each
// transition gives a -STEP_REWARD reward, except from the goal G that
// leads to a terminal state for all actions.

/*
 *   <----LENGTH---->
 *   ................ line number WIDTH-1
 *   ................
 *   ................
 *   ................ line number 0
 *   S##############G
 */

// The phase space is thus an integer, that we will define by an
// enum. Ths class Cliff handles the mapping to actual grid
// positions shown on the previous figure into an integer.


/**
 * Cliff-Walking  parameter handler.
 * @author <a href="mailto:Herve.Frezza-Buet@supelec.fr">Herve.Frezza-Buet@supelec.fr</a>
 */


/// \brief Enumeration describing the action
/// space for the Cliff-World problem
using CliffWorldAction=GridWorldAction;

struct CliffWorldReward
{
    /// \brief The reward received when  reaching the goal
    real_t  goal_reward() const {return    0.;}

    /// \brief The reward received when doing one step
    real_t step_reward() const {return   -1;}

    /// \brief The reward received when falling off the cliff
    real_t fall_reward() const {return -100.;}

    /// \brief The reward received
    real_t bump_reward() const {return  step_reward();}
};


/// \brief CliffWorld class models the Environment
/// for the cliff-world problem
class CliffWorld: public World<CliffWorldAction, int, real_t>
{

public:

    typedef World<CliffWorldAction, int, real_t>::state_t state_t;
    typedef World<CliffWorldAction, int, real_t>::reward_value_t reward_value_t;

    /// \brief the starting state of the world
    static const state_t  START = 0;

    /// \brief Constructor
    CliffWorld(uint_t xlength,  uint_t ylength);

    /// \brief Destructor
    ~CliffWorld();

    /// \brief Returns the state
    virtual state_t& sense()override final{return  agent_pos_;}

    /// \brief Transition to a new state by
    /// performing the given action
    virtual void step(const action_t&)override final;

    /// \brief Execute the start step
    void step_start(const CliffWorld::action_t& action);

    /// \brief Execute the stop step
    void step_goal(const CliffWorld::action_t& action);

    /// \brief Execute any other step
    void do_step(const CliffWorld::action_t& action);

    /// \brief Restart the world. This means that
    /// the stare of the world will be that of...
    void restart();

    /// \brief Set the state of the world
    void set_state(const state_t state);

    /// \brief Returns the reward associated
    /// with the last state transition
    virtual reward_value_t reward()const override final{return r_;}

    /// \brief The size of the world.  Namely,
    /// the total number of different positions in
    /// the grid
    uint_t size()const{return xlength_ * ylength_ + 2;}

private:

    /// \brief The length  of the world in the x direction
    uint_t xlength_;

    /// \brief The length of the world in the y direction
    uint_t ylength_;

    /// \brief The starting position
    uint_t start_{0};

    /// \brief The goal position
    uint_t goal_;

    /// \brief The current position
    /// of the agent in the world describes
    /// the state of the world. Initially this is simply
    /// the starting position
    state_t agent_pos_{0};

    /// \brief Object responsible for returning the rewards
    CliffWorldReward reward_;

    /// \brief The reward that the agent should recieve
    /// after executing an action
    reward_value_t r_;
};

}
}
}

#endif // CLIFF_WORLD_H
