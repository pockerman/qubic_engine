#include "cubic_engine/rl/worlds/gym_lunar_lander_world.h"
#include "cubic_engine/rl/gym_comm/communicator.h"

#include "cubic_engine/rl/gym_comm/requests.h"

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include <msgpack.hpp>

namespace cengine {
namespace rl {
namespace worlds {

struct GymLunarLanderWorld::GymLunarLanderWorldResetResponse
{
    std::vector<real_t> observation;
    MSGPACK_DEFINE_MAP(observation);
};

GymLunarLanderWorld::GymLunarLanderWorld(std::string&& version, gym::Communicator& comm)
    :
      GymWorldBase<std::vector<real_t>, uint_t>("LunarLander-", "v0", comm)
{}


std::tuple<GymLunarLanderWorld::state_t, real_t, bool, std::any>
GymLunarLanderWorld::step(const action_t&){

}

GymLunarLanderWorld::state_t
GymLunarLanderWorld::reset(){

    auto reset_param = std::make_shared<gym::ResetParam>();
    gym::Request<gym::ResetParam> reset_request("reset", reset_param);
    this->communicator_()->send_request(reset_request);

#ifdef USE_LOG
    kernel::Logger::log_info("Reset world " + this->name());
#endif

    this->state() = this->communicator_()->get_response<GymLunarLanderWorld::GymLunarLanderWorldResetResponse>()->observation;
    //this->update_current_state(obs);
    return this->state();

    // update the current state
    //current_state_ = comm_->get_response<gym::GymLunarLanderWorldResetResponse>()->x;
    //return current_state_;

}

void
GymLunarLanderWorld::build(bool reset_flag){

    if(reset_flag){
        reset();
    }
}




}
}
}
