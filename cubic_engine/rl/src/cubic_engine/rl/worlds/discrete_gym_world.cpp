#include "kernel/base/config.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include "cubic_engine/rl/worlds/discrete_gym_world.h"
#include "cubic_engine/rl/gym_comm/requests.h"

namespace cengine{
namespace rl{
namespace worlds{

DiscreteGymWorld::DiscreteGymWorld(gym::Communicator& comm)
    :
      comm_(&comm),
      current_state_(),
      is_built_(false),
      finished_(false)
{}

void
DiscreteGymWorld::build(const std::string& world_name, bool reset, bool get_info_params){

    auto make_param = std::make_shared<gym::MakeParam>();
    make_param->env_name = world_name;
    make_param->num_envs = 1;

    gym::Request<gym::MakeParam> make_request("make", make_param);
    comm_->send_request(make_request);

#ifdef USE_LOG
    kernel::Logger::log_info(comm_->get_response<gym::MakeResponse>()->result);
#endif

    if(get_info_params){
        n_states();
    }

    if(reset){
        restart();
    }

    world_name_ = world_name;
    is_built_ = true;
}

std::tuple<DiscreteGymWorld::state_t*, real_t, bool, std::any>
DiscreteGymWorld::step(const action_t& action){

    auto step_param = std::make_shared<gym::StepParam>();
    step_param->render = render_on_step_;
    step_param->action = static_cast<int>(action);

    gym::Request<gym::StepParam> step_request("step", step_param);
    comm_->send_request(step_request);

#ifdef USE_LOG
    kernel::Logger::log_info("Step in world " + world_name_ + " and action " + std::to_string(action));
#endif

    auto step_response = comm_->get_response<gym::DiscreteStepResponse>();
    current_state_[0] = step_response->observation;
    current_state_[1] = step_response->reward;
    current_state_[2] = step_response->done;

    finished_ = step_response->done;
    return std::make_tuple(&current_state_, step_response->reward, step_response->done, std::any());
}


DiscreteGymWorld::state_t*
DiscreteGymWorld::restart(){


    auto reset_param = std::make_shared<gym::ResetParam>();
    gym::Request<gym::ResetParam> reset_request("reset", reset_param);
    comm_->send_request(reset_request);

#ifdef USE_LOG
    kernel::Logger::log_info("Reset world " + world_name_);
#endif

    current_state_ = comm_->get_response<gym::ResetResponse>()->observation;
    return &current_state_;
}

uint_t
DiscreteGymWorld::n_states()const{

#ifdef KERNEL_DEBUG
    assert(is_built_ && "World is not built. Did you call build(world_name)?");
#endif

    if(!info_response_){

        gym::Request<gym::InfoParam> info_request("info", std::make_shared<gym::InfoParam>());
        comm_->send_request(info_request);
        info_response_ = comm_->get_response<gym::InfoResponse>();
    }

    return info_response_->observation_space_shape.size();
}


uint_t
DiscreteGymWorld::n_actions()const{

#ifdef KERNEL_DEBUG
    assert(is_built_ && "World is not built. Did you call build(world_name)?");
#endif

    if(!info_response_){

        gym::Request<gym::InfoParam> info_request("info", std::make_shared<gym::InfoParam>());
        comm_->send_request(info_request);
        info_response_ = comm_->get_response<gym::InfoResponse>();
    }

    return info_response_->action_space_shape.size();

}

}
}
}
