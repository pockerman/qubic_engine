#include "kernel/base/config.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include "cubic_engine/rl/worlds/gym_lunar_lander_world.h"
#include "cubic_engine/rl/gym_comm/communicator.h"
#include "cubic_engine/rl/gym_comm/requests.h"

#include <msgpack.hpp>
#include <iostream>

namespace cengine {
namespace rl {
namespace worlds {

struct GymLunarLanderWorld::ResetResponse
{
    std::vector<real_t> observation;
    std::string result;
    MSGPACK_DEFINE_MAP(observation, result);
};

struct GymLunarLanderWorld::StepResponse
{

    std::vector<real_t> observation;
    real_t reward;
    bool done;
    std::string result;
    MSGPACK_DEFINE_MAP(observation, reward, done, result);

};

struct GymLunarLanderWorld::InfoResponse
{

    std::vector<uint_t> observation_space_shape;
    std::vector<uint_t> action_space_shape;
    std::string result;
    MSGPACK_DEFINE_MAP(observation_space_shape, action_space_shape, result);
};

GymLunarLanderWorld::GymLunarLanderWorld(const std::string& version, gym::Communicator& comm)
    :
      GymWorldBase<std::vector<real_t>, uint_t>("LunarLander-" + version, version, comm)
{}


std::tuple<GymLunarLanderWorld::state_t, real_t, bool, std::any>
GymLunarLanderWorld::step(const action_t& action){

    auto step_param = std::make_shared<gym::StepRequest>();
    step_param->render = this->render_on_step();
    step_param->action = static_cast<int>(action);

    gym::Request<gym::StepRequest> step_request("step", step_param);
    comm_->send_request(step_request);

#ifdef USE_LOG
    kernel::Logger::log_info("Step in world " + this->name() + " and action " + std::to_string(action));
#endif

    auto response = this->communicator_()->get_response<GymLunarLanderWorld::StepResponse>();

#ifdef USE_LOG
    kernel::Logger::log_info("Response result " + response->result);
#endif

#ifdef KERNEL_DEBUG
        assert(response->result == "OK" && "Result from server is not OK");
#endif

   this->state() = response->observation;
    return std::make_tuple(response->observation, response->reward, response->done, std::any());

}

GymLunarLanderWorld::state_t
GymLunarLanderWorld::reset(){

#ifdef KERNEL_DEBUG
        assert(this->is_built() && "Environment is not built. Did you call build()?");
#endif

    auto reset_param = std::make_shared<gym::ResetRequest>();
    gym::Request<gym::ResetRequest> reset_request("reset", reset_param);
    this->communicator_()->send_request(reset_request);

#ifdef USE_LOG
    kernel::Logger::log_info("Reset world " + this->name());
#endif

    auto response = this->communicator_()->get_response<GymLunarLanderWorld::ResetResponse>();

#ifdef USE_LOG
    kernel::Logger::log_info("Response result " + response->result);
#endif

    std::cout<<"Reset world response is..."<<response->result<<std::endl;

#ifdef KERNEL_DEBUG
        assert(response->result == "OK" && "Result from server is not OK");
#endif

    this->state() = response->observation;
    return this->state();

}

void
GymLunarLanderWorld::build(bool reset_flag){

    auto make_param = std::make_shared<gym::MakeRequest>();
    make_param->env_name = this->name();
    make_param->num_envs = 1;

    gym::Request<gym::MakeRequest> make_request("make", make_param);
    this->communicator_()->send_request(make_request);

#ifdef USE_LOG
    kernel::Logger::log_info(this->communicator_()->get_response<gym::MakeResponse>()->result);
#endif

    this->make_is_built();

    if(reset_flag){
        reset();
    }
}




}
}
}
