#ifndef TD_BASE_H
#define TD_BASE_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
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

template<typename WorldTp>
class TDBase: private boost::noncopyable
{
public:

    ///
    /// \brief The type of the world
    ///
    typedef WorldTp world_t;

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
    virtual ~TDBase(){}

    ///
    /// \brief Train on the given world
    ///
    virtual void train();

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
    TDBase(uint_t max_iterations);

    ///
    /// \brief q_function_
    ///
    DynMat<real_t> q_function_;

    ///
    /// \brief The world used by the agent
    ///
    world_t* world_ptr_;

    ///
    /// \brief state_
    ///
    state_t* state_;

    ///
    /// \brief Flag indicating if the trainer has been initialized
    ///
    bool is_initialized_;

    ///
    /// \brief episode_controller_ The object controlling the
    /// episode iterations
    ///
    kernel::IterativeAlgorithmController episode_controller_;

    ///
    /// \brief epsilon_
    ///
    real_t epsilon_;

    ///
    /// \brief random_seed_
    ///
    uint_t random_seed_;

    ///
    /// \brief actions_before_episodes_ Actions performed before starting
    /// the episodes
    ///
    virtual void actions_before_episodes_(){}

    ///
    /// \brief actions_before_iterations_ Actions performed before starting
    /// the iterations
    ///
    virtual void actions_before_iterations_(){state_ = world_ptr_->restart();}

    ///
    /// \brief actions_after_iterations_ Actions performed after finishing
    /// the iterations
    ///
    virtual void actions_after_iterations_(){}

    ///
    /// \brief actions_after_episodes_ Actions performed after finishing
    /// the episodes
    ///
    virtual void actions_after_episodes_(){}

    ///
    /// \brief action_selection_policy. Select the action to perform
    /// By default applies an epsilon greedy policy
    ///
    virtual action_t action_selection_policy();

};

template<typename WorldTp>
TDBase<WorldTp>::TDBase(uint_t max_iterations)
    :
    q_function_(),
    world_ptr_(nullptr),
    is_initialized_(false),
    episode_controller_(max_iterations, std::numeric_limits<real_t>::max()),
    epsilon_(0.1),
    random_seed_(42)
{}

template<typename WorldTp>
void
TDBase<WorldTp>::initialize(world_t& world, reward_value_t val){

    q_function_.resize(world.n_states(), world.n_actions(), val);

    // finally set the world pointer
    world_ptr_ = &world;
    is_initialized_ = true;
}


template<typename WorldTp>
void
TDBase<WorldTp>::train(){

    if(!is_initialized_){
        throw std::logic_error("TD instance is not initialized");
    }

    //uint_t episode_counter = 0;
    actions_before_episodes_();
    while(episode_controller_.continue_iterations()){

        // for every iteration reset the environment
        actions_before_iterations_();

        // iterate for this episode
        step();

        // finish the episode
        actions_after_iterations_();
    }

    // finish th training
    actions_after_episodes_();

}

template<typename WorldTp>
typename TDBase<WorldTp>::action_t
TDBase<WorldTp>::action_selection_policy(){

    // Will be used to obtain a seed for the random number engine
    //std::random_device rd;

    std::seed_seq seed{random_seed_};

    // Standard mersenne_twister_engine seeded with rd()
    std::mt19937 gen(seed); //rd(random_seed_));
    std::uniform_real_distribution<> dis(0.0, 1.0);
    auto exp_exp_tradeoff = dis(seed);

    // do exploration by default
    auto action_idx = world_ptr_->sample_action();

    if( exp_exp_tradeoff > epsilon_ ){
          action_idx = static_cast<action_t>(kernel::row_argmax(q_function_, state_->get_id()));
    }

    return action_idx;
}


}
}

#endif
#endif // TD_H
