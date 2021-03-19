#include "cubic_engine/rl/worlds/grid_world.h"
#include "cubic_engine/rl/constant_environment_dynamics.h"

#include <vector>
#include <gtest/gtest.h>

namespace{

using namespace cengine;

class RewardProducer
{
public:

    typedef real_t value_t;

    /// construcotr
    RewardProducer(){}

    /// returns the reward for the goal
    real_t goal_reward()const{return 0.0;}

    /// returns the reward for the action
    /// at  state s when going to state sprime
    template<typename ActionTp, typename StateTp>
    real_t get_reward(const ActionTp& action,
                      const StateTp& s,
                      const StateTp& sprime)const{return 0.0;}

    template<typename ActionTp, typename StateTp>
    real_t get_reward(const ActionTp& action,
                      const StateTp& s)const{return 0.0;}

private:

};


}

TEST(TestGridWorld, Test_build) {


    using namespace cengine;
    rl::worlds::GridWorld<RewardProducer, cengine::rl::ConstantEnvironmentDynamics> environment;

    // use  a 2, 2 world
    environment.build(2,2);

    ASSERT_EQ(environment.n_states(), 4);

}

TEST(TestGridWorld, Test_save_world_as_json) {


    using namespace cengine;
    rl::worlds::GridWorld<RewardProducer, cengine::rl::ConstantEnvironmentDynamics> environment;

    // use  a 2, 2 world
    environment.build(2,2);
    try{
        environment.save_world_as_json("/home/alex/qi3/ce_rl_framework/cubic_engine/test_data/test_world.json");
   }
   catch(...){
        FAIL()<<"TestSaveJSON failed with unknonw exception";
    }

}

TEST(TestGridWorld, Test_load_world_from_json) {

    using namespace cengine;
    rl::worlds::GridWorld<RewardProducer, cengine::rl::ConstantEnvironmentDynamics> environment;
    environment.load_world_from_json("/home/alex/qi3/ce_rl_framework/cubic_engine/test_data/test_load_grid_world.json");

    ASSERT_EQ(environment.n_states(), 4);

}
