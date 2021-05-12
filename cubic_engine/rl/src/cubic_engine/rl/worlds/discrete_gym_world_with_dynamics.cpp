#include "cubic_engine/rl/worlds/discrete_gym_world_with_dynamics.h"
#include "cubic_engine/rl/gym_comm/requests.h"

namespace cengine{
namespace rl {
namespace worlds{

DiscreteGymWorldWithDynamics::DiscreteGymWorldWithDynamics(gym::Communicator& comm)
    :
      DiscreteGymWorld<uint_t>(comm)
{}

std::vector<std::tuple<real_t, DiscreteGymWorldWithDynamics::state_t, DiscreteGymWorldWithDynamics::reward_value_t, bool>>
DiscreteGymWorldWithDynamics::get_dynamics(const state_t& state, const action_t& action){

    auto dynamics_param = std::make_shared<gym::GetDynamicsParam>();
    dynamics_param->state = state;
    dynamics_param->action = action;

    gym::Request<gym::GetDynamicsParam> dynamics_request("dynamics", dynamics_param);
    this->comm()->send_request(dynamics_request);

#ifdef USE_LOG
    kernel::Logger::log_info("Dynamics request state " + std::to_string(state) + " and action " + std::to_string(action));
#endif

    auto dynamics_response = this->comm()->get_response<gym::DiscreteDynamicsResponse>();

    std::vector<std::tuple<real_t, DiscreteGymWorldWithDynamics::state_t,
    DiscreteGymWorldWithDynamics::reward_value_t, bool>> result;

    for(uint_t i =0; i<dynamics_response->probability.size(); ++i){
       result.push_back(std::make_tuple(dynamics_response->probability[i], dynamics_response->next_state[i],
                        dynamics_response->reward[i], dynamics_response->done[i]));
    }

    return result;

}

}
}
}
