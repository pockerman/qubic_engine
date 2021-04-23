#ifndef GREEDY_POLICY_H
#define GREEDY_POLICY_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/maths/matrix_utilities.h"
#include <vector>


namespace cengine {
namespace rl {

class GreedyPolicy{

public:

    ///
    /// \brief Constructor
    ///
    GreedyPolicy(const DynMat<real_t>& qtable);

    ///
    /// \brief Fuction call type. Take an observation as
    /// input and returns a vector of action probabilities.
    ///
    template<typename StateTp>
    auto get_probabilities(const StateTp& state) -> std::vector<real_t>;

    ///
    /// \brief Get the action index
    ///
    template<typename StateTp>
    auto get_action_index(const StateTp& state) const -> uint_t;

private:

    ///
    /// \brief qtable_
    ///
    const DynMat<real_t>* qtable_;
};

GreedyPolicy::GreedyPolicy(const DynMat<real_t>& qtable)
    :
      qtable_(&qtable)
{}

template<typename StateTp>
auto
GreedyPolicy::get_probabilities(const StateTp& state) -> std::vector<real_t>{
    std::vector<real_t> probabilities;
    return probabilities;
}

template<typename StateTp>
auto
GreedyPolicy::get_action_index(const StateTp& state) const -> uint_t{

    uint_t action_idx = kernel::get_row_max(*qtable_, state.get_id());
    return action_idx;

}

}

}

#endif // GREEDY_POLICY_H
