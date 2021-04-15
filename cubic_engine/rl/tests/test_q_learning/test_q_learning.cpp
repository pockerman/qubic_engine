#include "cubic_engine/rl/q_learning.h"
#include "cubic_engine/rl/worlds/grid_world.h"
#include "cubic_engine/rl/constant_environment_dynamics.h"
#include "cubic_engine/rl/reward_table.h"
#include <gtest/gtest.h>

namespace{

    using cengine::real_t;
    using cengine::rl::worlds::GridWorldAction;

    typedef cengine::rl::ConstantEnvironmentDynamics env_dynamics_t;
    typedef cengine::rl::RewardTable<GridWorldAction, real_t> reward_t;
    typedef cengine::rl::worlds::GridWorld<reward_t,env_dynamics_t>  world_t;
    cengine::rl::QLearningInput input;

}


TEST(QLearning, TestEmptyConstructor) {
   cengine::rl::QLearning<world_t> agent(input);
}
