#include "cubic_engine/rl/mc_control.h"
#include "cubic_engine/rl/worlds/grid_world.h"
#include "cubic_engine/rl/constant_environment_dynamics.h"
#include "cubic_engine/rl/rl_iterative_algo_input.h"
#include "cubic_engine/rl/reward_table.h"
#include "cubic_engine/rl/greedy_policy.h"
#include <gtest/gtest.h>

namespace{

    using cengine::real_t;
    using cengine::rl::GreedyPolicy;
    using cengine::rl::worlds::GridWorldAction;

    typedef cengine::rl::ConstantEnvironmentDynamics env_dynamics_t;
    typedef cengine::rl::RewardTable<GridWorldAction, real_t> reward_t;
    typedef cengine::rl::worlds::GridWorld<reward_t,env_dynamics_t>  world_t;
    cengine::rl::RLIterativeAlgoInput input;


}


TEST(MCControl, TestConstructor) {
   cengine::rl::MCControl<world_t, GreedyPolicy, GreedyPolicy> agent(input);
}
