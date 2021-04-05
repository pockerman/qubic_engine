#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/base/kernel_consts.h"
#include "cubic_engine/rl/worlds/grid_world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "cubic_engine/rl/synchronous_value_function_learning.h"
#include "cubic_engine/rl/reward_table.h"
#include "cubic_engine/rl/constant_environment_dynamics.h"

#include <iostream>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::rl::worlds::GridWorld;
using cengine::rl::worlds::GridWorldAction;
using cengine::rl::SyncValueFuncItr;
using cengine::rl::SyncValueFuncItrInput;
using cengine::rl::RewardTable;
using cengine::rl::ConstantEnvironmentDynamics;
using kernel::utilities::CSVWriter;

class RewardProducer
{
public:

    typedef real_t value_t;

    // construcotr
    RewardProducer();

    // returns the reward for the goal
    real_t goal_reward()const{return 0.0;}

    // returns the reward for the action
    // at  state s when going to state sprime
    template<typename ActionTp, typename StateTp>
    real_t get_reward(const ActionTp& action,
                      const StateTp& s,
                      const StateTp& sprime)const{
        return rewards_.get_reward(s.get_id(), action);
    }

    // returns the reward for the action
    // at  state s when going to state sprime
     template<typename ActionTp, typename StateTp>
     real_t get_reward(const ActionTp& action,
                          const StateTp& s)const{
            return rewards_.get_reward(s.get_id(), action);
     }

private:

    // table that holds the rewards
    RewardTable<GridWorldAction, real_t> rewards_;

    // setup the rewards
    void setup_rewards();
};

RewardProducer::RewardProducer()
    :
   rewards_()
{
    setup_rewards();
}

void
RewardProducer::setup_rewards(){

    rewards_.set_reward(0, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(0, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(0, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(0, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(1, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(1, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(1, GridWorldAction::WEST, -1.0);
    rewards_.set_reward(1, GridWorldAction::SOUTH, -1.0);

    rewards_.set_reward(2, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(2, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(2, GridWorldAction::WEST, -1.0);
    rewards_.set_reward(2, GridWorldAction::SOUTH, -1.0);

    rewards_.set_reward(4, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(4, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(4, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(4, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(5, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(5, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(5, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(5, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(6, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(6, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(6, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(6, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(7, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(7, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(7, GridWorldAction::WEST, -1.0);
    rewards_.set_reward(7, GridWorldAction::EAST, -1.0);

    rewards_.set_reward(8, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(8, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(8, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(8, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(9, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(9, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(9, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(9, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(10, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(10, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(10, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(10, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(11, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(11, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(11, GridWorldAction::WEST, -1.0);
    rewards_.set_reward(11, GridWorldAction::EAST, -1.0);

    rewards_.set_reward(13, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(13, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(13, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(13, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(14, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(14, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(14, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(14, GridWorldAction::WEST, -1.0);

    rewards_.set_reward(15, GridWorldAction::NORTH, -1.0);
    rewards_.set_reward(15, GridWorldAction::EAST, -1.0);
    rewards_.set_reward(15, GridWorldAction::SOUTH, -1.0);
    rewards_.set_reward(15, GridWorldAction::WEST, -1.0);
}



typedef GridWorld<RewardProducer, ConstantEnvironmentDynamics> world_t;
typedef world_t::state_t state_t;
 typedef world_t::action_t action_t;

const uint_t N_CELLS = 4;

}

int main(){

    using namespace example;

    try{

        auto policy = [](const action_t&, const state_t&){
          return 0.25;
        };

        RewardProducer rproducer;
        /*auto dynamics = [&rproducer](const state_t& s1, real_t,
                const state_t& s2, const action_t& action){
          return 0.25;
        };*/

        std::vector<real_t> rewards(1, -1.0);

        /// the world of the agent
        world_t world;
        world.build(N_CELLS, N_CELLS);
        world.get_dynamics_object().set_value(0.25);

        std::cout<<"Number of states: "<<world.n_states()<<std::endl;

        state_t start(15);
        state_t goal1(3);
        state_t goal2(12);

        world.append_goal(goal1);
        world.append_goal(goal2);

        // simulation parameters
        // number of episodes for the agent to learn.
        const uint_t N_ITERATIONS = 160;
        const real_t TOL = 0.001;
        const real_t GAMMA = 1.0;

        SyncValueFuncItrInput input={TOL, GAMMA, N_ITERATIONS, true};
        SyncValueFuncItr<world_t> learner(std::move(input));

        std::vector<real_t> row(2);
        learner.initialize(world, 0.0);

        world.restart(start);

        while(learner.continue_iterations()){

            std::cout<<"At iteration: "<<learner.get_current_iteration()<<std::endl;

            learner.step(policy);
            auto values = learner.get_values();

            for(auto c=0; c<values.size(); ++c){
                std::cout<<"Cell: "<<c<<" value: "<<values[c]<<std::endl;
            }   
        }    
    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
#else
#include <iostream>>
int main(){
    std::cerr<<"This examples requires RL support. Configure cubicengine with RL support."<<std::endl;
    return 1;
}
#endif
