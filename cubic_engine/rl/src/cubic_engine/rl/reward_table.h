#ifndef REWARD_TABLE_H
#define REWARD_TABLE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"

#if defined(__GNUC__) && (__GNUC___ > 7)
#include "magic_enum.hpp"
#else
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#endif

//#include "kernel/utilities/csv_file_writer.h"

#include <stdexcept>
#include <map>
#include <utility>
#include <vector>
#include <tuple>

namespace cengine{
namespace rl {

///
/// \brief Class to represent a reward table
///
template<typename ActionTp, typename RewardTp>
class RewardTable
{
public:

    typedef ActionTp action_t;
    typedef RewardTp reward_value_t;

    ///
    /// \brief Constructor
    ///
    RewardTable();

    ///
    /// \brief Add a reward
    ///
    void add_reward(uint_t state_id, const action_t& action, const reward_value_t& r);

    ///
    /// \brief Set the reward corresponding to the
    /// given state/action pair
    ///
    void set_reward(uint_t state_id, const action_t& action, const reward_value_t& r);

    ///
    /// \brief Returns the rewrad associated with the
    /// given state when performing the given action
    ///
    const reward_value_t get_reward(uint_t state_id, const action_t& action)const;

    /// \brief Returns the Action/Reward mapping for the
    /// given state id
    std::vector<std::pair<action_t, reward_value_t>> get_action_reward_mapping_for_state(uint_t sid)const;

    /// \brief Returns the Action with the maximum reward
    /// for the given state index
    action_t get_max_reward_action_at_state(uint_t sid)const;

    /// \brief Get the maximum reward for the state
    reward_value_t get_max_reward_at_state(uint_t sid)const;

    /// \brief Returns the total reward
    reward_value_t get_total_reward()const;

    /// \brief save the reward table in a CSV format
    void save_to_csv(const std::string& filename)const;

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
void
RewardTable<ActionTp, RewardTp>::set_reward(uint_t state_id, const action_t& action, const reward_value_t& r){
    add_reward(state_id, action, r);
}

template<typename ActionTp, typename RewardTp>
const typename RewardTable<ActionTp, RewardTp>::reward_value_t
RewardTable<ActionTp, RewardTp>::get_reward(uint_t state_id, const action_t& action)const{

    auto itr = reward_table_.find({state_id, action});

    if(itr == reward_table_.end()){
#if defined(__GNUC__) && (__GNUC___ > 7)
        throw std::logic_error("No such state-action pair " +
                               std::to_string(state_id) +
                               "-" +
                               magic_enum::enum_name(action));
#else
        throw std::logic_error("No such state-action pair " +
                               std::to_string(state_id) +
                               "-" +
                               worlds::to_string(action));
#endif

    }

    return itr->second;
}

template<typename ActionTp, typename RewardTp>
std::vector<std::pair<typename RewardTable<ActionTp, RewardTp>::action_t,
                      typename RewardTable<ActionTp, RewardTp>::reward_value_t>>
RewardTable<ActionTp, RewardTp>::get_action_reward_mapping_for_state(uint_t sid)const{

    std::vector<std::pair<typename RewardTable<ActionTp, RewardTp>::action_t,
            typename RewardTable<ActionTp, RewardTp>::reward_value_t>> values;

    auto begin = reward_table_.begin();
    auto end = reward_table_.end();

    for(; begin != end; ++begin){

        if(begin->first.first == sid){

            values.push_back({begin->first.second, begin->second});
        }
    }

    return values;
}

template<typename ActionTp, typename RewardTp>
typename RewardTable<ActionTp, RewardTp>::action_t
RewardTable<ActionTp, RewardTp>::get_max_reward_action_at_state(uint_t sid)const{

    auto actions = get_action_reward_mapping_for_state(sid);

    if(actions.empty()){
        throw std::logic_error("No actions found for state with id " +
                               std::to_string(sid));
    }

    auto max_reward = actions[0].second;
    auto action = actions[0].first;

    for(uint_t a=1; a<actions.size(); ++a){

        if(actions[a].second > max_reward){
            max_reward = actions[a].second;
            action = actions[a].first;
        }
    }

    return action;
}

template<typename ActionTp, typename RewardTp>
typename RewardTable<ActionTp, RewardTp>::reward_value_t
RewardTable<ActionTp, RewardTp>::get_max_reward_at_state(uint_t sid)const{

    auto actions = get_action_reward_mapping_for_state(sid);

    if(actions.empty()){
        throw std::logic_error("No actions found for state with id " +
                               std::to_string(sid));
    }

    auto max_reward = actions[0].second;

    for(uint_t a=1; a<actions.size(); ++a){

        if(actions[a].second > max_reward){
            max_reward = actions[a].second;
        }
    }

    return max_reward;

}

template<typename ActionTp, typename RewardTp>
typename RewardTable<ActionTp, RewardTp>::reward_value_t
RewardTable<ActionTp, RewardTp>::get_total_reward()const{

    typedef typename RewardTable<ActionTp, RewardTp>::reward_value_t reward_value_t;

    reward_value_t total = reward_value_t(0);

    auto itr = reward_table_.begin();
    auto itr_end = reward_table_.end();

    for(; itr != itr_end; ++itr){
        total += itr->second;
    }

    return total;

}

template<typename ActionTp, typename RewardTp>
void
RewardTable<ActionTp, RewardTp>::save_to_csv(const std::string& filename)const{

    /*kernel::CSVWriter writer(filename, ',', true);

    std::vector<std::string> col_names = {"StateId", "Action", "Reward"};
    writer.write_column_names(col_names, true);

    auto itr = reward_table_.begin();
    auto itr_end = reward_table_.end();

    for(; itr != itr_end; ++itr){

        std::tuple<uint_t, std::string, real_t> row= std::make_tuple(itr->first.first,
                                                                      worlds::to_string(itr->first.second),
                                                                     itr->second);

        writer.write_row(row);
    }*/
}

}
}

#endif // REWARD_TABLE_H
