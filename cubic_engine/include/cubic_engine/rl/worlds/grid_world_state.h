#ifndef GRID_WORLD_STATE_H
#define GRID_WORLD_STATE_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "kernel/base/kernel_consts.h"

#include <vector>
#include <map>

namespace cengine{
namespace rl {
namespace worlds {

///
/// \brief Small struct that describes
/// the state of a GridWorld instance
///
class GridWorldState
{
public:

    ///
    /// \brief A map from indexes to action type
    ///
    static GridWorldAction get_action_from_idx(uint_t i);

    ///
    /// \brief Constructor
    ///
    GridWorldState();

    ///
    /// \brief Costructor
    ///
    explicit GridWorldState(uint_t id);

    ///
    /// \brief Copy assignement
    ///
    GridWorldState& operator=(const GridWorldState& other);

    ///
    /// \brief The number of actions
    /// that the state allws
    ///
    uint_t n_actions()const{return 4;}

    ///
    /// \brief n_states. Returns the number of states
    ///
    uint_t n_states()const{return 4;}

    ///
    /// \brief is this action idx valid
    ///
    bool is_active_action(uint_t i)const;

    ///
    /// \brief Returns the id of the state
    ///
    uint_t get_id()const{return id_;}

    ///
    /// \brief Set the transitions for this state
    /// after executing the Action
    ///
    void set_transition(GridWorldAction action, GridWorldState* state);

    ///
    /// \brief Execute the action
    ///
    GridWorldState* execute_action(GridWorldAction action);

    ///
    /// \brief Returns the neighbor state for the given action
    ///
    const GridWorldState* execute_action(GridWorldAction action)const;

    ///
    /// \brief Select uniformly an active
    /// action
    ///
    GridWorldAction get_random_active_action()const;

    ///
    /// \brief Returns a vector with the available
    /// active actions
    ///
    const std::vector<GridWorldAction> get_active_actions()const;

    ///
    /// \brief Returns the a-th action
    ///
    GridWorldAction get_action(uint_t a)const;

    ///
    /// \brief Return the GridWorldAction required to
    /// get to the neighbor element. If the neighbor
    /// does not exist return GridWorldAction::INVALID_ACTION
    ///
    GridWorldAction get_action_for_neighbor(const GridWorldState& neighbor)const;

    ///
    /// \brief Returns a list with the transition
    /// states the agent can transition to from this
    /// state
    ///
    std::vector<const GridWorldState*> get_states()const;

    ///
    /// \brief get_state_transitions. Returns the state transitions
    ///
    std::map<GridWorldAction, GridWorldState*>& get_state_transitions(){return state_transitions_;}

    ///
    /// \brief get_state_transitions. Returns the state transitions
    ///
    const std::map<GridWorldAction, GridWorldState*>& get_state_transitions()const{return state_transitions_;}


private:

    ///
    /// \brief The id of the state
    ///
    uint_t id_;

    ///
    /// \brief The neighboring states that
    /// this state may transition to
    ///
    std::map<GridWorldAction, GridWorldState*> state_transitions_;

    ///
    /// \brief
    ///
    mutable uint_t previous_active_action_choice_{0};

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

#endif
#endif // GRID_WORLD_STATE_H
