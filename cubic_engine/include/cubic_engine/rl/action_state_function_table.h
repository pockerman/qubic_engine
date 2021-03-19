#ifndef ACTION_STATE_FUNCTION_TABLE_H
#define ACTION_STATE_FUNCTION_TABLE_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include <utility>
#include <unordered_map>

namespace cengine {
namespace rl {

///
/// \brief The ActionStateFunctionTable class.
/// A nested dictionary that maps state -> (action -> action-value).
///
template<typename StateTp, typename ActionTp>
class ActionStateFunctionTable
{
public:

    ///
    /// \brief state_t The type of the state
    ///
    typedef StateTp state_t;

    ///
    /// \brief action_t. The type of the action
    ///
    typedef ActionTp action_t;

    ///
    /// \brief key_t.
    ///
    typedef std::pair<action_t, real_t> value_t;

    ///
    /// \brief ActionStateFunctionTable. Constructor
    ///
    ActionStateFunctionTable();

private:

    ///
    /// \brief state_action_values_. The map that holds
    /// state-action pairs
    ///
    std::unordered_map<state_t, value_t> state_action_values_;
};

}

}

#endif
#endif // ACTION_STATE_FUNCTION_TABLE_H
