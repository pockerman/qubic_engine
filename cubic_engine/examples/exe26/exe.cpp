#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/array_utils.h"
#include "kernel/maths/matrix_utilities.h"

#include <cmath>
#include <utility>
#include <tuple>
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include <limits>
#include <map>

namespace example
{
using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using kernel::CSVWriter;

// max number of cars in each location
const int MAX_CARS = 20;

// max number of cars to move during night
const uint_t MAX_MOVE_OF_CARS = 5;

// expectation for rental requests in first location
const uint_t RENTAL_REQUEST_FIRST_LOC = 3;

// expectation for rental requests in second location
const uint_t RENTAL_REQUEST_SECOND_LOC = 4;

// expectation for # of cars returned in first location
const int RETURNS_FIRST_LOC = 3;

// expectation for # of cars returned in second location
const int RETURNS_SECOND_LOC = 2;

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

// cache for the Poisson values
std::map<int, real_t> poisson_cache;

real_t poisson_pmf(int  k, real_t lambda) {
    return std::exp(k * std::log(lambda) - std::lgamma(k + 1.0) - lambda);
  }

real_t poisson_probability(int n, int lam ){

    auto key = n * 10 + lam;
    auto itr = poisson_cache.find(key);
    if(itr == poisson_cache.end()){
        poisson_cache[key] = poisson_pmf(n, lam);
    }

    return poisson_cache[key];
}

real_t expected_return(int state[], int action, DynMat<real_t>& state_value, bool constant_returned_cars){

    // initailize total return
    real_t returns = 0.0;

    // cost for moving cars
    returns -= MOVE_CAR_COST * std::abs(action);

    // moving cars
    int NUM_OF_CARS_FIRST_LOC = std::min(state[0] - action, MAX_CARS);
    int NUM_OF_CARS_SECOND_LOC = std::min(state[1] + action, MAX_CARS);

    // go through all possible rental requests
    for(int rental_request_first_loc=0;  rental_request_first_loc<POISSON_UPPER_BOUND; ++rental_request_first_loc){
        for(int rental_request_second_loc=0;  rental_request_second_loc<POISSON_UPPER_BOUND; ++rental_request_second_loc){

            // probability for current combination of rental requests
            auto prob = poisson_probability(rental_request_first_loc, RENTAL_REQUEST_FIRST_LOC);
            prob *= poisson_probability(rental_request_second_loc, RENTAL_REQUEST_SECOND_LOC);

            auto num_of_cars_first_loc = NUM_OF_CARS_FIRST_LOC;
            auto num_of_cars_second_loc = NUM_OF_CARS_SECOND_LOC;

            // valid rental requests should be less than actual # of cars
            auto valid_rental_first_loc = std::min(num_of_cars_first_loc, rental_request_first_loc);
            auto valid_rental_second_loc = std::min(num_of_cars_second_loc, rental_request_second_loc);

            // get credits for renting
            auto reward = (valid_rental_first_loc + valid_rental_second_loc) * RENTAL_CREDIT;
            num_of_cars_first_loc -= valid_rental_first_loc;
            num_of_cars_second_loc -= valid_rental_second_loc;

            if( constant_returned_cars){
                // get returned cars, those cars can be used for renting tomorrow
                auto returned_cars_first_loc = RETURNS_FIRST_LOC;
                auto returned_cars_second_loc = RETURNS_SECOND_LOC;
                num_of_cars_first_loc = std::min(num_of_cars_first_loc + returned_cars_first_loc, MAX_CARS);
                num_of_cars_second_loc = std::min(num_of_cars_second_loc + returned_cars_second_loc, MAX_CARS);
                returns += prob * (reward + DISCOUNT * state_value(num_of_cars_first_loc, num_of_cars_second_loc));
            }
            else{
                for(int returned_cars_first_loc=0; returned_cars_first_loc<POISSON_UPPER_BOUND; ++returned_cars_first_loc){
                    for( int returned_cars_second_loc=0; returned_cars_second_loc<POISSON_UPPER_BOUND; ++returned_cars_second_loc){

                        auto prob_return = poisson_probability(
                            returned_cars_first_loc, RETURNS_FIRST_LOC) * poisson_probability(returned_cars_second_loc, RETURNS_SECOND_LOC);
                        auto num_of_cars_first_loc_ = std::min(num_of_cars_first_loc + returned_cars_first_loc, MAX_CARS);
                        auto num_of_cars_second_loc_ = std::min(num_of_cars_second_loc + returned_cars_second_loc, MAX_CARS);
                        auto prob_ = prob_return * prob;
                        returns += prob_ * (reward + DISCOUNT *
                                            state_value(num_of_cars_first_loc_, num_of_cars_second_loc_));
                    }
                }
           }
          }
         }
    return returns;

}


void simulate(bool constant_returned_cars){

    DynMat<real_t> value(MAX_CARS + 1, MAX_CARS + 1, 0.);
    DynMat<int> policy(MAX_CARS + 1, MAX_CARS + 1, 0);

    // how many iteration have we performed
    uint_t iterations = 0;

    while(true){

        std::string filename="policy_" + std::to_string(iterations) + ".csv";
        CSVWriter policywriter(filename, ',', true);

        for(uint_t r=0; r<policy.rows(); ++r){
            auto row = kernel::get_row(policy, r);
            policywriter.write_row(row);
        }

        std::cout<<"Iteration: "<<iterations<<std::endl;

        // policy evaluation (in-place)
        while(true) {

           // a copy of the current value function
           auto old_value = value;

           for(int i=0; i<(MAX_CARS + 1); ++i){
               for(int j=0; j<(MAX_CARS + 1); ++j){

                   int local_state[] = {i,j};
                   auto new_state_value = expected_return(local_state, policy(i,j),
                                                          value, constant_returned_cars);
                   value(i,j) = new_state_value;
               }
           }

           auto  max_value_change = max(abs(old_value - value));
           std::cout<<"max value change: "<<max_value_change<<std::endl;
           if(max_value_change < 1e-4){
                    break;
           }

        }

        // policy improvement
        bool policy_stable = true;

        for(int i=0; i<(MAX_CARS + 1); ++i){
            for(int j=0; j<(MAX_CARS + 1); ++j){

                auto old_action = policy(i, j);
                std::vector<real_t> action_returns;

                for(uint_t a=0; a<11; ++a){

                    auto action = actions[a];
                    if((0 <= action <= i) || (-j <= action <= 0)){
                        int local_state[] = {i,j};
                        action_returns.push_back(expected_return(local_state, action,
                                                                 value, constant_returned_cars));
                    }
                    else{
                        action_returns.push_back(std::numeric_limits<real_t>::min());
                    }
                }

                auto new_action = actions[kernel::utils::arg_max(action_returns)];
                policy(i, j) = new_action;
                if( policy_stable && old_action != new_action){
                        policy_stable = false;
                }
            }
        }

        std::cout<<"Policy stable: "<<std::boolalpha<<policy_stable<<std::endl;
        if(policy_stable){

            CSVWriter valuewriter("value.csv", ',', true);

            for(uint_t r=0; r<value.rows(); ++r){
                auto row = kernel::get_row(value, r);
                valuewriter.write_row(row);
            }

            break;
        }

        iterations += 1;
}
}


}

int main(){


    using namespace example;


    try{
        simulate(true);
    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
