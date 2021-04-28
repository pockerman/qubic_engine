#include "cubic_engine/rl/mc_control.h"
#include "cubic_engine/rl/worlds/grid_world.h"
#include "cubic_engine/rl/constant_environment_dynamics.h"
#include "cubic_engine/rl/rl_iterative_algo_input.h"
#include "cubic_engine/rl/reward_table.h"
#include "cubic_engine/rl/greedy_policy.h"

#include<iostream>

namespace example{

    using cengine::real_t;
    using cengine::rl::GreedyPolicy;
    using cengine::rl::worlds::GridWorldAction;

    typedef cengine::rl::ConstantEnvironmentDynamics env_dynamics_t;
    typedef cengine::rl::RewardTable<GridWorldAction, real_t> reward_t;
    typedef cengine::rl::worlds::GridWorld<reward_t,env_dynamics_t>  world_t;


}


int main(){


    using namespace example;

    try{

        cengine::rl::RLIterativeAlgoInput input;
        cengine::rl::MCControl<world_t, GreedyPolicy, GreedyPolicy> agent(input);
    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
