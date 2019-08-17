#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/csv_file_writer.h"
#include <boost/any.hpp>

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
using cengine::DynMat;

const uint_t NUM_ITRS = 1000;
const uint_t GRID_SIZE = 4;
const int REWARD_SIZE = -1;
const real_t GAMMA = 1.0;


std::vector<real_t> flatten(const std::vector<std::vector<real_t>>& vals){

    std::vector<real_t> rslt;
    rslt.reserve(GRID_SIZE*GRID_SIZE);

    for(uint_t i=0; i<vals.size(); ++i){
        for(uint_t j=0; j<vals[i].size(); ++j){
            rslt.push_back(vals[i][j]);
        }
    }
    return rslt;
}


struct Action
{
  int i;
  int j;
};

struct State
{
  uint_t i;
  uint_t j;
};

static const State terminal_state_1={0, 0};
static const State terminal_state_2={GRID_SIZE-1, GRID_SIZE-1};

bool operator == (const State& s1, const State& s2 ){

    return ((s1.i == s2.i) && (s1.j == s2.j));
}

 class PolicyIteration
 {
 public:

     PolicyIteration();

     // iterate over the states and actions
     void iterate(const std::vector<State>& states, const std::vector<Action>&  actions );

     // calculate the reward given the state and the action
     std::pair<State, int> action_reward_function(const State& state, const Action& action);

 private:

     // 2D map
     DynMat<real_t> value_map_;

     // the gamma
     real_t gamma_;

     // number of iterations to perform
     uint_t num_itrs_;

 };

PolicyIteration::PolicyIteration()
    :
      value_map_(GRID_SIZE, GRID_SIZE, 0.0),
      gamma_(GAMMA),
      num_itrs_(NUM_ITRS)
{}

std::pair<State, int>
PolicyIteration::action_reward_function(const State& state, const Action& action){


    if( state == terminal_state_1 || state == terminal_state_2){
        return std::make_pair(state, 0);
    }

    auto reward = REWARD_SIZE;
    State new_state = {state.i + action.i, state.j + action.j};

    if((new_state.i == -1) || (new_state.j == -1) ||
       (new_state.i == 4) || (new_state.j == 4))
        new_state = state;

    return std::make_pair(new_state, reward);
}

void
PolicyIteration::iterate(const std::vector<State>& states, const std::vector<Action>&  actions){

    const uint_t ACTIONS_SIZE = actions.size();
    std::vector<std::vector<real_t>> deltas;

    std::cout<<"Starting with value map: "<<std::endl;
    std::cout<<value_map_<<std::endl;

    kernel::CSVWriter writer("pi_deltas", kernel::CSVWriter::default_delimiter(), true);
    std::vector<std::string> names{"Iteration", "Delta"};
    writer.write_column_names(names);

    for(uint_t itr=0; itr<num_itrs_; ++itr){

        DynMat<real_t> copy_value_map = value_map_;
        std::vector<real_t> delta_state;

        for(uint_t  state_idx=0; state_idx < states.size(); ++state_idx ){

            real_t weighted_reward = 0.0;
            const State& state = states[state_idx];

            for(uint_t  action_idx=0; action_idx < ACTIONS_SIZE; ++action_idx){

                const Action& action = actions[action_idx];
                State final_pos = State();
                int reward;
                std::tie(final_pos, reward) = action_reward_function(state, action);
                weighted_reward += (1.0/ACTIONS_SIZE)*(reward + (gamma_*value_map_(final_pos.i, final_pos.j)));
            }

            delta_state.push_back(std::abs(copy_value_map(state.i, state.j) - weighted_reward));
            copy_value_map(state.i, state.j) = weighted_reward;
        }

        value_map_ = copy_value_map;
        deltas.push_back(delta_state);

        std::vector<real_t> row(delta_state.size()+1);
        row[0] = itr;
        for(uint_t i=0; i<delta_state.size(); ++i){
           row[i+1] = delta_state[i];
        }

        writer.write_row(row);

        if( itr == 0 || itr == 1 || itr == 2 || itr == 99 || itr == num_itrs_-1){
            std::cout<<"Iteration: "<<itr<<std::endl;
            std::cout<<value_map_<<std::endl;
        }
    }
}

class MonteCarlo
{

public:

    MonteCarlo();

    // iterate over the states and actions
    void iterate(const std::vector<State>& states, const std::vector<Action>&  actions );

    std::vector<std::vector<boost::any>> generate_episode(const std::vector<State>& states, const std::vector<Action>&  actions);

private:

    // 2D map
    DynMat<real_t> value_map_;

    // the gamma
    real_t gamma_;

    // number of iterations to perform
    uint_t num_itrs_;

    struct Returns
    {

        int i=-1;
        int j=-1;
        std::vector<real_t> vals;
    };

    bool state_in(const State& state, std::vector<boost::any>& vals)const;
};

MonteCarlo::MonteCarlo()
    :
      value_map_(GRID_SIZE, GRID_SIZE, 0.0),
      gamma_(GAMMA),
      num_itrs_(NUM_ITRS)
{}


bool
MonteCarlo::state_in(const State& state, std::vector<boost::any>& vals)const{

    auto state_query = boost::any_cast<State>(vals[0]);
    return (state == state_query);
}

std::vector<std::vector<boost::any> >
MonteCarlo::generate_episode(const std::vector<State>& states, const std::vector<Action>&  actions){

    //randomly choose a state
    //std::default_random_engine state_generator;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<uint_t> state_distribution(0, states.size()-1);
    uint_t state_idx = state_distribution(gen);  // generates number in the range 0..states.size()

    State init_state = states[state_idx];
    std::vector<std::vector<boost::any> > episode;

    while(true){

        if( init_state == terminal_state_1 || init_state == terminal_state_2){
            return episode;
        }

        //std::default_random_engine action_generator;
        std::uniform_int_distribution<uint_t> action_distribution(0, actions.size()-1);
        uint_t action_idx = action_distribution(gen);
        auto action = actions[action_idx];

        State final_state = {init_state.i + action.i, init_state.j + action.j};

        if((final_state.i == -1) || (final_state.j == -1) ||
           (final_state.i == 4) || (final_state.j == 4))
            final_state = init_state;

        std::vector<boost::any> iteration;
        iteration.push_back(init_state);
        iteration.push_back(action);
        iteration.push_back(REWARD_SIZE);
        iteration.push_back(final_state);
        episode.push_back(iteration);
        init_state = final_state;
    }
}

void
MonteCarlo::iterate(const std::vector<State>& states, const std::vector<Action>&  actions ){

    std::vector<Returns> returns;

    for(int i=0; i< GRID_SIZE; ++i){
       for(int j=0; j< GRID_SIZE; ++j){
           returns.push_back({i, j, std::vector<real_t>()});
       }
    }

    std::vector<std::vector<real_t>> deltas(GRID_SIZE);

    for(uint_t i=0; i<GRID_SIZE; ++i){
        deltas[i].reserve(GRID_SIZE);

        for(uint_t j=0; j<GRID_SIZE; ++j){
            deltas[i].push_back(0.0);
        }
    }

    kernel::CSVWriter writer("mc_deltas", kernel::CSVWriter::default_delimiter(), true);
    std::vector<std::string> names{"Iteration", "Delta"};
    writer.write_column_names(names);

    for(uint_t itr=0; itr<num_itrs_; ++itr){

        std::cout<<"At iteration: "<<itr+1<<std::endl;
        auto episode = generate_episode(states, actions);
        auto G = 0;

        auto begin = episode.rbegin();
        auto end   = episode.rend();
        uint_t e = 0;

        for(; begin != end; ++begin){

            auto episode_vals = *begin;
            auto val = boost::any_cast<int>(episode_vals[2]);
            G = gamma_*G + val;

            auto state = boost::any_cast<State>(episode_vals[0]);

            if(!state_in(state,  episode[e++])){

                auto itr = std::find_if(returns.begin(), returns.end(),
                                        [&state](const Returns& r){
                    return (r.i == state.i && r.j == state.j);
                });

                if(itr != returns.end()){

                    auto& return_val = *itr;
                    return_val.vals.push_back(G);
                    auto new_val = std::accumulate(return_val.vals.begin(), return_val.vals.end(), 0.0)/return_val.vals.size();
                    deltas[return_val.i][return_val.j] = std::abs(value_map_(return_val.i, return_val.j) - new_val);
                    value_map_(return_val.i, return_val.j) = new_val;
                }
            }
        }

        std::vector<real_t> row(GRID_SIZE*GRID_SIZE+1);
        auto flatten_values = flatten(deltas);
        row[0] = itr;
        for(uint_t i=0; i<flatten_values.size(); ++i){
                row[i+1] = flatten_values[i];
        }

        writer.write_row(row);
    }
}

}

int main(int argc, char** argv) {

   using namespace exe;
   uint_t method=1;

   std::vector<Action> actions;
   actions.push_back({-1, 0});
   actions.push_back({1,  0});
   actions.push_back({0,  1});
   actions.push_back({0, -1});

   std::vector<State> states;

   for(uint_t i=0; i< GRID_SIZE; ++i){
      for(uint_t j=0; j< GRID_SIZE; ++j){
          states.push_back({i, j});
      }
   }


   if(method == 0){
     std::cout<<"Policy iteration..."<<std::endl;
     PolicyIteration policy_itr;
     policy_itr.iterate(states, actions);
   }

   if(method == 1){
        std::cout<<"Monte Carlo iteration..."<<std::endl;
        MonteCarlo monte_carlo;
        monte_carlo.iterate(states, actions);
   }
    
   return 0;
}

