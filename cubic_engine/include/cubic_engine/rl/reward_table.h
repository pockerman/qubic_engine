#ifndef REWARD_TABLE_H
#define REWARD_TABLE_H

#include "cubic_engine/base/cubic_engine_types.h"

#include <stdexcept>
#include <map>
#include <utility>

namespace cengine{
namespace rl {


/// \brief Class to represent a reward table
template<typename ActionTp, typename RewardTp>
class RewardTable
{
public:

    typedef ActionTp action_t;
    typedef RewardTp reward_value_t;

    /// \brief Constructor
    RewardTable();

    /// \brief Add a reward
    void add_reward(uint_t state_id, const action_t& action, const reward_value_t& r);

    /// \brief Returns the rewrad associated with the
    /// given state when performing the given action
    const reward_value_t get_reward(uint_t state_id, const action_t& action)const;

private:

    /// \brief A map that holds the reward values given
    /// a state and an action
    std::map<std::pair<uint_t, action_t>, reward_value_t> reward_table_;

};

template<typename ActionTp, typename RewardTp>
RewardTable<ActionTp, RewardTp>::RewardTable()
    :
    reward_table_()
{}

template<typename ActionTp, typename RewardTp>
void
RewardTable<ActionTp, RewardTp>::add_reward(uint_t state_id,
                                            const typename RewardTable<ActionTp, RewardTp>::action_t& action,
                                            const RewardTable<ActionTp, RewardTp>::reward_value_t& r){

    reward_table_.insert_or_assign(std::make_pair(state_id, action), r);

}

template<typename ActionTp, typename RewardTp>
const typename RewardTable<ActionTp, RewardTp>::reward_value_t
RewardTable<ActionTp, RewardTp>::get_reward(uint_t state_id, const action_t& action)const{

    auto itr = reward_table_.find({state_id, action});

    if(itr == reward_table_.end()){
        throw std::logic_error("No such state-action pair");
    }

    return itr->second;
}

}
}

#endif // REWARD_TABLE_H
