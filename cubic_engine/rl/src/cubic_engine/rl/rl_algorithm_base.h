#ifndef RL_ALGORITHM_BASE_H
#define RL_ALGORITHM_BASE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/rl_iterative_algo_input.h"
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
class RLAlgorithmBase: private boost::noncopyable
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
    virtual ~RLAlgorithmBase()=default;

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
    /// \brief get_discount_factor
    ///
    real_t get_discount_factor()const{return input_.discount_factor;}

    ///
    /// \brief set_discount_factor
    /// \param gamm
    ///
    void set_discount_factor(real_t gamma){input_.discount_factor = gamma;}

    ///
    /// \brief get_epsilon
    ///
    real_t get_epsilon()const{return input_.epsilon;}

    ///
    /// \brief set_epsilon
    ///
    void set_epsilon(real_t eps){input_.epsilon = eps;}

    ///
    /// \brief get_random_seed
    ///
    uint_t get_random_seed()const{return input_.random_seed;}

    ///
    /// \brief set_random_seed
    ///
    void set_random_seed(uint_t seed){input_.random_seed = seed;}

    ///
    /// \brief get_total_episodes
    ///
    uint_t get_total_episodes()const{return input_.total_episodes;}

    ///
    /// \brief get_total_itrs_per_episode
    ///
    uint_t get_total_itrs_per_episode()const{return input_.max_num_iterations;}

    ///
    /// \brief get_learning_rate
    ///
    real_t get_learning_rate()const{return input_.learning_rate;}

protected:

    ///
    /// \brief Constructor
    ///
    RLAlgorithmBase(const RLIterativeAlgoInput& input);

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
    /// \brief input_ Constants used by the algorithms
    ///
    RLIterativeAlgoInput input_;

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

};

template<typename WorldTp>
RLAlgorithmBase<WorldTp>::RLAlgorithmBase(const RLIterativeAlgoInput& input)
    :
    world_ptr_(nullptr),
    state_(nullptr),
    is_initialized_(false),
    episode_controller_(input.total_episodes, std::numeric_limits<real_t>::max()),
    input_(input)
{}


template<typename WorldTp>
void
RLAlgorithmBase<WorldTp>::train(){

    if(!is_initialized_){
        throw std::logic_error("TD instance is not initialized");
    }

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

}

}

#endif // RL_ALGORITHM_BASE_H
