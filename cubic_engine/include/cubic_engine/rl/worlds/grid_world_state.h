#ifndef GRID_WORLD_STATE_H
#define GRID_WORLD_STATE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "kernel/base/kernel_consts.h"

#include <map>

namespace cengine{
namespace rl {
namespace worlds {

/// \brief Small struct that describes
/// the state of a GridWorld instance
class GridWorldState
{
public:

    /// \brief A map from indexes to action type
    static GridWorldAction get_action_from_idx(uint_t i);

    /// \brief Constructor
    GridWorldState();

    /// \brief Costructor
    explicit GridWorldState(uint_t id);

    /// \brief Copy assignement
    GridWorldState& operator=(const GridWorldState& other);

    /// \brief The number of actions
    /// that the state allws
    uint_t n_actions()const{return 4;}

    /// \brief Returns the id of the state
    uint_t get_id()const{return id_;}

    /// \brief Set the transitions for this state
    /// after executing the Action
    void set_transition(GridWorldAction action, GridWorldState* state);

    /// \brief Execute the action
    GridWorldState* execute_action(GridWorldAction action);

private:

    /// \brief The id of the state
    uint_t id_;

    /// \brief The neighboring states that
    /// this state may transition to
    std::map<GridWorldAction, GridWorldState*> state_transitions_;

};


inline
bool operator==(const GridWorldState& s1, const GridWorldState& s2){
    return s1.get_id() == s2.get_id();
}

inline
bool operator!=(const GridWorldState& s1, const GridWorldState& s2){
    return !(s1 == s2);
}

}

}
}

#endif // GRID_WORLD_STATE_H
