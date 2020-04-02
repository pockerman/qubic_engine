#include "cubic_engine/rl/worlds/cliff_world.h"

namespace cengine{
namespace rl{
namespace worlds{

CliffWorld::CliffWorld(uint_t xlength,  uint_t ylength)
    :
      World<CliffWorldAction, int, real_t>(),
      xlength_(xlength),
      ylength_(ylength),
      goal_(xlength * ylength + 1)
{}

CliffWorld::~CliffWorld()
{}

void
CliffWorld::restart(){
   set_state(CliffWorld::START);
}


void
CliffWorld::set_state(const state_t state){
    agent_pos_ = state;
}


void
CliffWorld::step(const CliffWorld::action_t& action){

    const static uint_t START = start_;

    /// switch of the current state
    /// of the world
    if(agent_pos_ == start_){
        step_start(action);
    }
    else if(agent_pos_ == goal_){
        step_goal(action);
    }
    else{
        do_step(action);
    }
}

void
CliffWorld::step_start(const CliffWorld::action_t& action){

    switch(action) {
      case CliffWorldAction::NORTH:
        agent_pos_ = 1;
        r_ = reward_.step_reward();
        break;
      case CliffWorldAction::SOUTH:
      case CliffWorldAction::WEST:
        r_ = reward_.bump_reward();
        break;
      case CliffWorldAction::EAST:
        r_ = reward_.fall_reward();
        break;
      default:
        std::ostringstream ostr;
        ostr << "cliff_walking::Simulator::stepStart(" << static_cast<int>(action) << ")";
        //throw BadAction(ostr.str());
    }
}

void
CliffWorld::step_goal(const CliffWorld::action_t& action) {

      r_ = reward_.goal_reward();
      //throw rl::exception::Terminal("Transition from goal");
    }

void
CliffWorld::do_step(const CliffWorld::action_t& action){

    // Easier index
    uint_t s = agent_pos_ - 1;

     switch(action) {

        case CliffWorldAction::NORTH:

         // not upper wall
          if(s / xlength_ < ylength_ -1) {
            s += xlength_;
            r_ = reward_.step_reward();
          }
          else{
            r_ = reward_.bump_reward();
          }

          break;

        case CliffWorldAction::SOUTH:

         // not on the edge
          if(s / xlength_ > 0) {
            s -= xlength_;
            r_ = reward_.step_reward();
          }
          else if(s == 0) {

            // On start again
            s = start_ - 1;
            r_ = reward_.bump_reward();
          }
          else if(s == xlength_-1) {

            // remember that we will do s++...
            s = goal_ - 1;
            r_ = reward_.step_reward();
          }
          else {
            s = start_-1;
            r_ = reward_.fall_reward();
          }

          break;

        case CliffWorldAction::EAST:

         // Not on right wall
          if(s % xlength_ < xlength_ - 1) {
            r_ = reward_.step_reward();
            s++;
          }
          else{
            r_ = reward_.bump_reward();
          }
          break;
        case CliffWorldAction::WEST:

         // Not on left wall
          if(s % xlength_ != 0) {
            r_ = reward_.step_reward();
            s--;
          }
          else{
            r_ = reward_.bump_reward();
          }
          break;
        default:
          std::ostringstream ostr;
          ostr << "cliff_walking::Simulator::timeStep(" << static_cast<int>(action) << ")";
          //throw BadAction(ostr.str());
     }

     agent_pos_ = s+1;

}


}
}
}
