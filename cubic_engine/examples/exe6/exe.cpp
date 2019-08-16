#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/csv_file_writer.h"
#include <cmath>
#include <utility>
#include <tuple>
#include <iostream>

namespace exe
{
using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;

const uint_t NUM_ITRS = 1000;
const uint_t GRID_SIZE = 4;
const int REWARD_SIZE = -1;
const real_t GAMMA = 1.0;


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

    static const State terminal_state_1={0, 0};
    static const State terminal_state_2={GRID_SIZE-1, GRID_SIZE-1};

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

    kernel::CSVWriter writer("deltas", kernel::CSVWriter::default_delimiter(), true);
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

}

int main(int argc, char** argv) {

   using namespace exe;

   PolicyIteration policy_itr;

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

   policy_itr.iterate(states, actions);
    
   return 0;
}

