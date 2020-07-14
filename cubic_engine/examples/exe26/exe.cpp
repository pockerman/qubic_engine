#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/csv_file_writer.h"
#include "cubic_engine/rl/worlds/cliff_world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "cubic_engine/rl/tabular_sarsa_learning.h"
#include "cubic_engine/rl/reward_table.h"

#include <cmath>
#include <utility>
#include <tuple>
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include <limits>

namespace example
{
using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::rl::worlds::GridWorldAction;
using cengine::rl::SarsaTableLearning;
using cengine::rl::SarsaLearningInput;
using cengine::rl::RewardTable;
using kernel::CSVWriter;

// max number of cars in each location
const uint_t MAX_CARS = 20;

// max number of cars to move during night
const uint_t MAX_MOVE_OF_CARS = 5;

// expectation for rental requests in first location
const uint_t RENTAL_REQUEST_FIRST_LOC = 3;

// expectation for rental requests in second location
const uint_t RENTAL_REQUEST_SECOND_LOC = 4;

// expectation for # of cars returned in first location
const uint_t RETURNS_FIRST_LOC = 3;

// expectation for # of cars returned in second location
const uint_t RETURNS_SECOND_LOC = 2;

// discount factor
const real_t DISCOUNT = 0.9;

// credit earned by a car
const real_t RENTAL_CREDIT = 10;

// cost of moving a car
const real_t MOVE_CAR_COST = 2;

// all possible actions
int actions[] = {-5, -4, -3, -2, -1,  0,  1,  2,  3,  4,  5};

// An up bound for poisson distribution
// If n is greater than this value, then the probability
// of getting n is truncated to 0
const uint_t POISSON_UPPER_BOUND = 11;

real_t expected_return(state, int action, state_value, constant_returned_cars){

    // initailize total return
    real_t returns = 0.0;

    // cost for moving cars
    returns -= MOVE_CAR_COST * std::abs(action);

    # moving cars
    int NUM_OF_CARS_FIRST_LOC = std::min(state[0] - action, MAX_CARS);
    int NUM_OF_CARS_SECOND_LOC = std::min(state[1] + action, MAX_CARS);

    # go through all possible rental requests
    for rental_request_first_loc in range(POISSON_UPPER_BOUND):
        for rental_request_second_loc in range(POISSON_UPPER_BOUND):
            # probability for current combination of rental requests
            prob = poisson_probability(rental_request_first_loc, RENTAL_REQUEST_FIRST_LOC) * \
                poisson_probability(rental_request_second_loc, RENTAL_REQUEST_SECOND_LOC)

            num_of_cars_first_loc = NUM_OF_CARS_FIRST_LOC
            num_of_cars_second_loc = NUM_OF_CARS_SECOND_LOC

            # valid rental requests should be less than actual # of cars
            valid_rental_first_loc = min(num_of_cars_first_loc, rental_request_first_loc)
            valid_rental_second_loc = min(num_of_cars_second_loc, rental_request_second_loc)

            # get credits for renting
            reward = (valid_rental_first_loc + valid_rental_second_loc) * RENTAL_CREDIT
            num_of_cars_first_loc -= valid_rental_first_loc
            num_of_cars_second_loc -= valid_rental_second_loc

            if constant_returned_cars:
                // get returned cars, those cars can be used for renting tomorrow
                returned_cars_first_loc = RETURNS_FIRST_LOC
                returned_cars_second_loc = RETURNS_SECOND_LOC
                num_of_cars_first_loc = min(num_of_cars_first_loc + returned_cars_first_loc, MAX_CARS)
                num_of_cars_second_loc = min(num_of_cars_second_loc + returned_cars_second_loc, MAX_CARS)
                returns += prob * (reward + DISCOUNT * state_value[num_of_cars_first_loc, num_of_cars_second_loc])
            else:
                for returned_cars_first_loc in range(POISSON_UPPER_BOUND):
                    for returned_cars_second_loc in range(POISSON_UPPER_BOUND):
                        prob_return = poisson_probability(
                            returned_cars_first_loc, RETURNS_FIRST_LOC) * poisson_probability(returned_cars_second_loc, RETURNS_SECOND_LOC)
                        num_of_cars_first_loc_ = min(num_of_cars_first_loc + returned_cars_first_loc, MAX_CARS)
                        num_of_cars_second_loc_ = min(num_of_cars_second_loc + returned_cars_second_loc, MAX_CARS)
                        prob_ = prob_return * prob
                        returns += prob_ * (reward + DISCOUNT *
                                            state_value[num_of_cars_first_loc_, num_of_cars_second_loc_])
    return returns

}


void simulate(){

    DynMat<real_t> value(MAX_CARS + 1, MAX_CARS + 1, 0.);
    DynMat<int> policy(MAX_CARS + 1, MAX_CARS + 1, -1);

    // how many iteration have we performed
    uint_t iterations = 0;


    while(true){

        // policy evaluation (in-place)
        while(true) {

           // a copy of the current value function
           auto old_value = value;

           for(uint_t i=0; i<(MAX_CARS + 1); ++i){
               for(uint_t j=0; j<(MAX_CARS + 1); ++j){

                   auto new_state_value = expected_return([i, j], policy(i,j), value, constant_returned_cars);
                   value(i,j) = new_state_value;
               }

               auto  max_value_change = std::fabs(old_value - value).max();
               std::cout<<"max value change: "<<max_value_change<<std::endl;
               if(max_value_change < 1e-4){
                    break;
               }
           }
        }

        // policy improvement
        bool policy_stable = true;

        for(uint_t i=0; i<(MAX_CARS + 1); ++i){
            for(uint_t j=0; j<(MAX_CARS + 1); ++j){

                auto old_action = policy(i, j);
                std::vector<real_t> action_returns;

                for(uint_t a=0; a<11; ++a){

                    auto action = actions[a];
                    if((0 <= action <= i) || (-j <= action <= 0)){
                        action_returns.push_back(expected_return([i, j], action, value, constant_returned_cars));
                    }
                    else{
                        action_returns.push_back(std::numeric_limits<real_t>::min());
                    }
                }

                auto new_action = actions[np.argmax(action_returns)]
                policy(i, j) = new_action;
                if( policy_stable && old_action != new_action){
                        policy_stable = false;
                }
            }
        }

        if(policy_stable){
            break;
        }




}

}

int main(){





    try{

        typedef CliffWorld world_t;
        typedef world_t::state_t state_t;

        /// the world of the agent
        CliffWorld world;
        world.create_world();

        std::cout<<"Number of states: "<<world.n_states()<<std::endl;

        state_t start(0);
        state_t goal(11);

        /// simulation parameters
        /// number of episodes for the agent to learn.
        const uint_t N_ITERATIONS = 500;
        const real_t ETA = 0.1;
        const real_t EPSILON = 0.1;
        const real_t GAMMA = 1.0;
        const real_t PENALTY = -100.0;

        SarsaLearningInput qinput={ETA, EPSILON, GAMMA, true, true};
        SarsaTableLearning<world_t> sarsalearner(std::move(qinput));

        CSVWriter writer("agent_rewards.csv", ',', true);
        writer.write_column_names({"Episode", "Reward"}, true);

        std::vector<real_t> row(2);
        sarsalearner.initialize(world, PENALTY);

        auto& table = sarsalearner.get_table();
        table.save_to_csv("table_rewards" + std::to_string(0) + ".csv");

        for(uint_t episode=0; episode < N_ITERATIONS; ++episode){

            std::cout<<"At episode: "<<episode<<std::endl;
            world.restart(start, goal);
            auto result = sarsalearner.train(goal);

            /// the total reward the agent obtained in this episode
            auto reward = result.total_reward;
            writer.write_row(std::make_tuple(episode, reward));
            std::cout<<"At episode: "<<episode<<" total reward: "<<reward<<std::endl;

            if(episode == N_ITERATIONS - 1){
                auto& table = sarsalearner.get_table();
                table.save_to_csv("table_rewards" + std::to_string(episode) + ".csv");
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
