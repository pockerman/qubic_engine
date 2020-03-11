#ifndef CLIFF_WORLD_H
#define CLIFF_WORLD_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/world.h"
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

enum class CliffWorldAction{NORTH=0, SOUTH=1,
                            EAST=2, WEST=3};


class CliffWorld: public World<CliffWorldAction, int, int>
{

public:

    typedef World<CliffWorldAction, int, int>::state_t state_t;
    typedef World<CliffWorldAction, int, int>::reward_t reward_t;

    /// \brief Returns the state
    virtual state_t& sense()override final;

    /// \brief Transition to a new state by
    /// performing the given action
    virtual void step(const action_t&)override final;




};

}
}
}

#endif // CLIFF_WORLD_H
