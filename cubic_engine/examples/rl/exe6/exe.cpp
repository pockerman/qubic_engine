#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/csv_file_writer.h"
#include "cubic_engine/rl/worlds/grid_world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "cubic_engine/rl/q_learning.h"
#include "cubic_engine/rl/reward_table.h"
#include "cubic_engine/rl/constant_environment_dynamics.h"

#include <cmath>
#include <utility>
#include <tuple>
#include <iostream>
#include <random>
#include <algorithm>

namespace exe
{

using cengine::uint_t;
using cengine::real_t;
using cengine::rl::worlds::GridWorld;
using cengine::rl::worlds::GridWorldAction;
using cengine::rl::QTableLearning;
using cengine::rl::QLearningInput;
using cengine::rl::RewardTable;
using cengine::rl::ConstantEnvironmentDynamics;
using kernel::CSVWriter;
const uint_t N_CELLS = 7;
const real_t PENALTY = -100;


class RewardProducer
{
public:

    typedef real_t value_t;

    /// construcotr
    RewardProducer();

    /// returns the reward for the goal
    real_t goal_reward()const{return 0.0;}

    /// returns the reward for the action
    /// at  state s when going to state sprime
    template<typename ActionTp, typename StateTp>
    real_t get_reward(const ActionTp& action,
                      const StateTp& s,
                      const StateTp& sprime)const;

    template<typename ActionTp, typename StateTp>
    real_t get_reward(const ActionTp& action,
                      const StateTp& s)const;

private:

    /// table that holds the rewards
    RewardTable<GridWorldAction, real_t> rewards_;

    /// setup the rewards
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

    for(uint_t i=0; i<N_CELLS*N_CELLS; ++i){

        if(i<7){

            if(i != 6 && i !=0 ){
                rewards_.add_reward(i, GridWorldAction::EAST,  PENALTY);
                rewards_.add_reward(i, GridWorldAction::NORTH,  -1.0);
                rewards_.add_reward(i, GridWorldAction::WEST,  PENALTY);
            }
            else{

               if(i == 0){
                   rewards_.add_reward(i, GridWorldAction::EAST,  PENALTY);
               }
               else{
                   rewards_.add_reward(i, GridWorldAction::WEST,  PENALTY);
               }

               rewards_.add_reward(i, GridWorldAction::NORTH,  PENALTY);
            }
        }
        else if(i>=42){

            if(i != 42 && i != 48 ){
                rewards_.add_reward(i, GridWorldAction::EAST,  PENALTY);

                if(i != 43){
                    rewards_.add_reward(i, GridWorldAction::SOUTH,  -1.0);
                }
                else{
                   rewards_.add_reward(i, GridWorldAction::SOUTH,  0.0);
                }

                rewards_.add_reward(i, GridWorldAction::WEST,  PENALTY);
            }
            else{
               rewards_.add_reward(i, GridWorldAction::SOUTH,  PENALTY);

               if( i == 42){
                    rewards_.add_reward(i, GridWorldAction::EAST,  PENALTY);
               }

               if(i == 48){
                   rewards_.add_reward(i, GridWorldAction::WEST,  PENALTY);
               }
            }


        }
        else{

            static const uint_t arwest[]={7,14,21,28, 35};
            static const uint_t areast[]={13,20,27,34, 41};

            if(std::find(&arwest[0],
                         &arwest[ sizeof(arwest)/sizeof(uint_t) ],
                         i) != &arwest[ sizeof (arwest)/sizeof(uint_t) ]){

                rewards_.add_reward(i, GridWorldAction::SOUTH,  PENALTY);
                rewards_.add_reward(i, GridWorldAction::NORTH,  PENALTY);

                if(i != 35){
                    rewards_.add_reward(i, GridWorldAction::EAST,  -1.0);
                }
                else{
                   rewards_.add_reward(i, GridWorldAction::EAST,  0.0);
                }
            }
            else if(std::find(&areast[0],
                              &areast[ sizeof(areast)/sizeof(uint_t) ],
                              i) != &areast[ sizeof (areast)/sizeof(uint_t) ]){

                rewards_.add_reward(i, GridWorldAction::SOUTH,  PENALTY);
                rewards_.add_reward(i, GridWorldAction::NORTH,  PENALTY);

                if(i != 13){
                    rewards_.add_reward(i, GridWorldAction::WEST,  -1.0);
                }
                else{
                   rewards_.add_reward(i, GridWorldAction::WEST,  0.0);
                }
            }
            else{

                static uint_t short_path_1[] ={8, 15, 22, 29, 36};
                static uint_t short_path_2[] ={37, 38, 39, 40};
                static uint_t short_path_3[] ={33, 26, 13, 12};
                static uint_t short_path_4[] ={9, 10, 11};

                if(std::find(&short_path_1[0],
                             &short_path_1[ sizeof(short_path_1)/sizeof(uint_t) ],
                                         i) != &short_path_1[ sizeof (short_path_1)/sizeof(uint_t) ]){


                     if(i != 36){
                         rewards_.add_reward(i, GridWorldAction::NORTH,  -2.0);
                         rewards_.add_reward(i, GridWorldAction::WEST,  PENALTY);

                         if(i != 8){
                             rewards_.add_reward(i, GridWorldAction::EAST,  -3.);
                             rewards_.add_reward(i, GridWorldAction::SOUTH,  -1.0);
                         }
                         else{
                             rewards_.add_reward(i, GridWorldAction::EAST,  -1.0);
                             rewards_.add_reward(i, GridWorldAction::SOUTH,  PENALTY);
                         }
                     }
                     else{
                        rewards_.add_reward(i, GridWorldAction::NORTH,  PENALTY);
                        rewards_.add_reward(i, GridWorldAction::WEST,  PENALTY);
                        rewards_.add_reward(i, GridWorldAction::EAST,  -1.0);
                        rewards_.add_reward(i, GridWorldAction::SOUTH,  -1.0);
                     }
                }
                else if(std::find(&short_path_2[0],
                                  &short_path_2[ sizeof(short_path_2)/sizeof(uint_t) ],
                                              i) != &short_path_2[ sizeof (short_path_2)/sizeof(uint_t) ]){


                    rewards_.add_reward(i, GridWorldAction::NORTH,  PENALTY);

                    if(i != 40){
                        rewards_.add_reward(i, GridWorldAction::SOUTH,  -3.0);
                        rewards_.add_reward(i, GridWorldAction::EAST,  -1.0);
                        rewards_.add_reward(i, GridWorldAction::WEST,  -2.0);
                    }
                    else{
                        rewards_.add_reward(i, GridWorldAction::SOUTH,  -1.0);
                        rewards_.add_reward(i, GridWorldAction::EAST,  PENALTY);
                        rewards_.add_reward(i, GridWorldAction::WEST,  -2.0);
                    }

                }
                else if(std::find(&short_path_3[0],
                                  &short_path_3[ sizeof(short_path_3)/sizeof(uint_t) ],
                                              i) != &short_path_3[ sizeof (short_path_3)/sizeof(uint_t) ]){


                    rewards_.add_reward(i, GridWorldAction::NORTH,  -2.0);
                    rewards_.add_reward(i, GridWorldAction::EAST,  PENALTY);

                    if(i != 12){
                        rewards_.add_reward(i, GridWorldAction::SOUTH,  -1.0);
                        rewards_.add_reward(i, GridWorldAction::WEST,  -3.0);
                    }
                    else{
                        rewards_.add_reward(i, GridWorldAction::SOUTH,  PENALTY);
                        rewards_.add_reward(i, GridWorldAction::WEST,  -2.0);
                    }
                }
                else if(std::find(&short_path_4[0],
                                  &short_path_4[ sizeof(short_path_4)/sizeof(uint_t) ],
                                              i) != &short_path_4[ sizeof (short_path_4)/sizeof(uint_t) ]){

                    rewards_.add_reward(i, GridWorldAction::NORTH,  -3.);
                    rewards_.add_reward(i, GridWorldAction::SOUTH,  PENALTY);
                    rewards_.add_reward(i, GridWorldAction::WEST,  -2.0);

                    if(i==11){
                       rewards_.add_reward(i, GridWorldAction::EAST,  0.);
                    }
                    else{
                        rewards_.add_reward(i, GridWorldAction::EAST,  -1.);
                    }
                }
                else{

                    if(i == 30 || i == 31 || i == 32){

                        rewards_.add_reward(i, GridWorldAction::NORTH,  -1.0);
                        rewards_.add_reward(i, GridWorldAction::SOUTH,  -3.0);

                        if(i == 32){
                            rewards_.add_reward(i, GridWorldAction::EAST,  -1.0);
                        }
                        else{
                            rewards_.add_reward(i, GridWorldAction::EAST,  -3.0);
                        }

                        if(i == 30){
                            rewards_.add_reward(i, GridWorldAction::WEST,  -1.0);
                        }
                        else{
                            rewards_.add_reward(i, GridWorldAction::WEST,  -3.0);
                        }
                    }
                    else if (i == 23 || i == 24 || i == 25) {

                        rewards_.add_reward(i, GridWorldAction::NORTH,  -3.0);
                        rewards_.add_reward(i, GridWorldAction::SOUTH,  -3.0);

                        if(i == 25){
                            rewards_.add_reward(i, GridWorldAction::EAST,  -1.0);
                        }
                        else{
                            rewards_.add_reward(i, GridWorldAction::EAST,  -3.0);
                        }

                        if(i == 23){
                            rewards_.add_reward(i, GridWorldAction::WEST,  -1.0);
                        }
                        else{
                            rewards_.add_reward(i, GridWorldAction::WEST,  -3.0);
                        }
                    }
                    else{
                        rewards_.add_reward(i, GridWorldAction::NORTH,  -3.0);
                        rewards_.add_reward(i, GridWorldAction::SOUTH,  -1.0);

                        if(i == 18){
                            rewards_.add_reward(i, GridWorldAction::EAST,  -1.0);
                        }
                        else{
                            rewards_.add_reward(i, GridWorldAction::EAST,  -3.0);
                        }

                        if(i == 16){
                            rewards_.add_reward(i, GridWorldAction::WEST,  -1.0);
                        }
                        else{
                            rewards_.add_reward(i, GridWorldAction::WEST,  -3.0);
                        }
                    }
                }
            }
        }

    }
}

template<typename ActionTp, typename StateTp>
real_t
RewardProducer::get_reward(const ActionTp& action,
                           const StateTp& s,
                           const StateTp& sprime)const{

    return rewards_.get_reward(s.get_id(), action);

}

template<typename ActionTp, typename StateTp>
real_t
RewardProducer::get_reward(const ActionTp& action,
                           const StateTp& s)const{

    return rewards_.get_reward(s.get_id(), action);

}

typedef GridWorld<RewardProducer, ConstantEnvironmentDynamics> world_t;
typedef world_t::state_t state_t;

}

int main() {

    using namespace exe;

    /// the world of the agent
    world_t world;

    // build a 6x6
    world.build(6, 6);

    std::cout<<"Number of states: "<<world.n_states()<<std::endl;

    state_t start(30);
    state_t goal(5);

    /// simulation parameters
    const real_t EPSILON = 0.1;

    QLearningInput qinput;
    qinput.discount_factor = 1.0;
    qinput.epsilon = 1.0;
    qinput.max_epsilon = 1.0;
    qinput.min_epsilon = 0.01;
    qinput.show_iterations = true;
    qinput.total_episodes = 15;
    qinput.max_num_iterations = 100;
    qinput.use_decay = true;

    QTableLearning<world_t> qlearner(std::move(qinput));
    qlearner.initialize(world, PENALTY);

    // train the agent
    qlearner.train();

    // now that we train let's play
    auto& qtable = qlearner.get_q_function();
    world.restart(start, goal);

    auto counter = 0;
    /*while(!stop){

        auto& state = world.get_current_state();
        std::cout<<"At state: "<<state.get_id()<<std::endl;

        if(state == goal){
            std::cout<<"State: "<<state.get_id()<<" is the goal"<<std::endl;
            break;
        }

        /// get the action with the maximum value
        auto action = qtable.get_max_reward_action_at_state(state.get_id());

        std::cout<<"Maximum reward action: "<<cengine::rl::worlds::to_string(action)<<std::endl;
        std::cout<<"MAximum reward is: "<<qtable.get_max_reward_at_state(state.get_id())<<std::endl;
        world.execute_action(action);
        counter++;
    }*/

   return 0;
}
#else
#include <iostream>
int main(){

    std::cerr<<"You need to configure CubicEngine with RL support in order to run this example"<<std::endl;
    return 0;
}
#endif

