#ifndef TD_BASE_H
#define TD_BASE_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/rl_iterative_algo_input.h"
#include "cubic_engine/rl/rl_algorithm_base.h"
#include "kernel/utilities/iterative_algorithm_controller.h"
#include "kernel/maths/matrix_utilities.h"

#include <boost/noncopyable.hpp>
#include <functional>
#include <limits>
#include <random>

namespace cengine {
namespace rl{


///
///\brief Class for learning state-value functions
/// using TD
///
template<typename WorldTp>
class TDBase: public RLAlgorithmBase<WorldTp>
{
public:

    ///
    /// \brief The type of the world
    ///
    typedef typename RLAlgorithmBase<WorldTp>::world_t world_t;

    ///
    /// \brief The type of the action
    ///
    typedef typename world_t::action_t action_t;

    ///
    /// \brief The type of the reward
    ///
    typedef typename world_t::reward_value_t reward_value_t;

    ///
    /// \brief The type of the state
    ///
    typedef typename world_t::state_t state_t;

    ///
    /// \brief ~TDBase Destructor
    ///
    virtual ~TDBase()=default;

    ///
    /// \brief step. Performs the iterations for
    /// one training episode
    ///
    virtual void step()=0;

    ///
    /// \brief Initialize the underlying data structures
    ///
    virtual void initialize(world_t& world, reward_value_t val);

    ///
    /// \brief Returns the learnt Qfunction
    ///
    const DynMat<real_t>& get_q_function()const{return q_function_;}

    ///
    /// \brief Returns the learnt Qfunction
    ///
    DynMat<real_t>& get_q_function(){return q_function_;}

protected:

    ///
    /// \brief Constructor
    ///
    TDBase(const RLIterativeAlgoInput& input);

    ///
    /// \brief q_function_
    ///
    DynMat<real_t> q_function_;

    ///
    /// \brief action_selection_policy. Select the action to perform
    /// By default applies an epsilon greedy policy
    ///
    virtual action_t action_selection_policy(const state_t& state);

private:

};

template<typename WorldTp>
TDBase<WorldTp>::TDBase(const RLIterativeAlgoInput& input)
    :
    RLAlgorithmBase<WorldTp>(input),
    q_function_()
{}

template<typename WorldTp>
void
TDBase<WorldTp>::initialize(world_t& world, reward_value_t val){

    q_function_.resize(world.n_states(), world.n_actions(), val);

    // finally set the world pointer
    this->world_ptr_ = &world;
    this->state_ = nullptr;
    this->is_initialized_ = true;
}


template<typename WorldTp>
typename TDBase<WorldTp>::action_t
TDBase<WorldTp>::action_selection_policy(const state_t& state){

    std::seed_seq seed{this->input_.random_seed};
    std::mt19937 rd(this->input_.random_seed);

    // Standard mersenne_twister_engine seeded with rd()
    std::mt19937 gen(seed); //rd(random_seed_));
    std::uniform_real_distribution<> dis(0.0, 1.0);
    auto exp_exp_tradeoff = dis(rd);

    // do exploration by default
    auto action_idx = this->world_ptr_->sample_action();

    if( exp_exp_tradeoff > this->input_.epsilon ){
          action_idx = static_cast<action_t>(kernel::row_argmax(q_function_, state.get_id()));
    }

    return action_idx;
}

}
}

#endif
#endif // TD_H
