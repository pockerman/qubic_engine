#ifndef DISCRETE_GYM_WORLD_H
#define DISCRETE_GYM_WORLD_H

#include "kernel/base/config.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include "kernel/base/kernel_consts.h"
#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/discrete_world.h"
#include "cubic_engine/rl/dummy_environment_dynamics.h"
#include "cubic_engine/rl/gym_comm/communicator.h"
#include "cubic_engine/rl/gym_comm/requests.h"

#include <vector>
#include <any>
#include <string>
#include <memory>
#include <tuple>

namespace cengine {
namespace rl {
namespace worlds {


template<typename StateTp>
class DiscreteGymWorld
{
public:

    ///
    /// \brief action_t The action type for the world
    ///
    typedef uint_t action_t;

    ///
    /// \brief state_t The state type
    ///
    typedef StateTp state_t;

    ///
    /// \brief reward_value_t The reward value type
    ///
    typedef real_t reward_value_t;

    ///
    /// \brief invalid_action Indicate an invalid action on the world
    /// Useful for uniform initialization when action is unknown
    ///
    static constexpr action_t INVALID_ACTION = kernel::KernelConsts::INVALID_SIZE_TYPE;

    ///
    /// \brief DiscreteGymWorld Constructor
    /// \param comm
    ///
    DiscreteGymWorld(gym::Communicator& comm);

    ///
    /// \brief Build the OpenAI Gym world
    ///
    virtual  void build(const std::string& world_name, bool reset=true, bool get_info_params=true);

    ///
    /// \brief n_states. Returns the number of states
    ///
    uint_t n_states()const;

    ///
    /// \brief n_actions. Max number of actions per state
    ///
    virtual uint_t n_actions()const;

    ///
    /// \brief Transition to a new state by
    /// performing the given action. It returns a tuple
    /// with the following information
    /// arg1: An observation of the environment.
    /// arg2: Amount of reward achieved by the previous action.
    /// arg3: Flag indicating whether it’s time to reset the environment again.
    /// Most (but not all) tasks are divided up into well-defined episodes,
    /// and done being True indicates the episode has terminated.
    /// (For example, perhaps the pole tipped too far, or you lost your last life.)
    /// arg4: The type depends on the subclass overriding this function
    /// diagnostic information useful for debugging. It can sometimes be useful for
    /// learning (for example, it might contain the raw probabilities behind the environment’s last state change).
    /// However, official evaluations of your agent are not allowed to use this for learning.
    ///
    virtual std::tuple<state_t, real_t, bool, std::any> step(const action_t&);

    ///
    /// \brief sample_action. Sample an action from
    /// the allowed action space of the world
    ///
    virtual action_t sample_action()const;

    ///
    /// \brief returns the current state of the world
    ///
    const state_t& get_current_state()const noexcept{return current_state_; }

    ///
    /// \brief restart. Restart the world and
    /// return the starting state
    ///
    state_t reset();

    ///
    /// \brief Returns true if the world is finished
    ///
    bool is_finished()const noexcept{return finished_;}

    ///
    /// \brief Returns the reward for the given state
    /// and the given actions
    ///
    reward_value_t get_reward(const state_t& state, const action_t& action)const;
    
protected:

    gym::Communicator* comm(){return comm_;}

private:

    ///
    /// \brief comm_ The object responsible for the
    /// communication with Gym OpenAI
    ///
    gym::Communicator* comm_;

    ///
    /// \brief current_state_. The current world state
    ///
    state_t current_state_;

    ///
    /// \brief is_built_ Flag indicating if the world has
    /// been built
    ///
    bool is_built_;

    ///
    /// \brief world_name_
    ///
    std::string world_name_;

    ///
    /// \brief Flag indicating if the world
    /// has the current_state_ and goal_ state equal
    ///
    bool finished_;

    ///
    /// \brief render_on_step_
    ///
    bool render_on_step_;

    ///
    /// \brief info_response_ Struc that holds basic informatin
    /// for the environement
    mutable std::unique_ptr<gym::InfoResponse> info_response_;


};

template<typename StateTp>
DiscreteGymWorld<StateTp>::DiscreteGymWorld(gym::Communicator& comm)
    :
      comm_(&comm),
      current_state_(),
      is_built_(false),
      finished_(false),
      render_on_step_(false)
{}

template<typename StateTp>
typename DiscreteGymWorld<StateTp>::action_t
DiscreteGymWorld<StateTp>::sample_action()const{
    return action_t();
}


template<typename StateTp>
void
DiscreteGymWorld<StateTp>::build(const std::string& world_name, bool restart, bool get_info_params){

    auto make_param = std::make_shared<gym::MakeParam>();
    make_param->env_name = world_name;
    make_param->num_envs = 1;

    gym::Request<gym::MakeParam> make_request("make", make_param);
    comm_->send_request(make_request);

#ifdef USE_LOG
    kernel::Logger::log_info(comm_->get_response<gym::MakeResponse>()->result);
#endif

    world_name_ = world_name;
    is_built_ = true;

    if(get_info_params){
        n_states();
    }

    if(restart){
        reset();
    }
}

template<typename StateTp>
std::tuple<typename DiscreteGymWorld<StateTp>::state_t, real_t, bool, std::any>
DiscreteGymWorld<StateTp>::step(const action_t& action){

    auto step_param = std::make_shared<gym::StepParam>();
    step_param->render = render_on_step_;
    step_param->action = static_cast<int>(action);

    gym::Request<gym::StepParam> step_request("step", step_param);
    comm_->send_request(step_request);

#ifdef USE_LOG
    kernel::Logger::log_info("Step in world " + world_name_ + " and action " + std::to_string(action));
#endif

    auto step_response = comm_->get_response<gym::DiscreteStepResponse>();
    //current_state_[0] = step_response->observation;
    //current_state_[1] = step_response->reward;
    //current_state_[2] = step_response->done;

    finished_ = step_response->done;
    return std::make_tuple(step_response->observation, step_response->reward, step_response->done, std::any());
}

template<typename StateTp>
typename DiscreteGymWorld<StateTp>::state_t
DiscreteGymWorld<StateTp>::reset(){


    auto reset_param = std::make_shared<gym::ResetParam>();
    gym::Request<gym::ResetParam> reset_request("reset", reset_param);
    comm_->send_request(reset_request);

#ifdef USE_LOG
    kernel::Logger::log_info("Reset world " + world_name_);
#endif

    // update the current state
    current_state_ = comm_->get_response<gym::DiscreteResetResponse>()->x;
    return current_state_;
}


template<typename StateTp>
uint_t
DiscreteGymWorld<StateTp>::n_states()const{

#ifdef KERNEL_DEBUG
    assert(is_built_ && "World is not built. Did you call build(world_name)?");
#endif

    if(!info_response_){

        gym::Request<gym::InfoParam> info_request("info", std::make_shared<gym::InfoParam>());
        comm_->send_request(info_request);
        info_response_ = comm_->get_response<gym::InfoResponse>();
#ifdef USE_LOG
    kernel::Logger::log_info("Update info response");
#endif
    }

    return info_response_->observation_space_size;
}


template<typename StateTp>
uint_t
DiscreteGymWorld<StateTp>::n_actions()const{

#ifdef KERNEL_DEBUG
    assert(is_built_ && "World is not built. Did you call build(world_name)?");
#endif

    if(!info_response_){

        gym::Request<gym::InfoParam> info_request("info", std::make_shared<gym::InfoParam>());
        comm_->send_request(info_request);
        info_response_ = comm_->get_response<gym::InfoResponse>();
#ifdef USE_LOG
    kernel::Logger::log_info("Update info response");
#endif
    }

    return info_response_->action_space_shape[0]; //size();

}


}

}

}

#endif // DISCRETE_GYM_WORLD_H
