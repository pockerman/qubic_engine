#include "cubic_engine/base/cubic_engine_types.h"
#include <cmath>
#include <utility>

namespace exe
{
using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;

const uint_t NUM_ITRS = 100;
const uint_t GRID_SIZE = 4;
const int REWARD_SIZE = -1;


struct Action
{
  uint_t i;
  uint_t j;
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

     void iterate(const std::vector<State>& states, const std::vector<Action>&  actions );

     // calculate the reward given the state and the action
     std::pair<State, uint_t> action_reward_function(const State& state, const Action& action);

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
      value_map_()
{}

std::pair<State, uint_t>
PolicyIteration::action_reward_function(const State& state, const Action& action){

    static const State terminal_state_1={0, 0};
    static const State terminal_state_2={GRID_SIZE-1, GRID_SIZE-1};

    if( state == terminal_state_1 || state == terminal_state_2){
        return std::make_pair(state, 0);
    }

    auto reward = REWARD_SIZE;
}

void
PolicyIteration::iterate(const std::vector<State>& states, const std::vector<Action>&  actions){

    const uint_t ACTIONS_SIZE = actions.size();
    std::vector<std::vector<real_t>> deltas;

    for(uint_t itr=0; itr<num_itrs_; ++itr){

        DynMat<real_t> copy_value_map = value_map_;
        std::vector<real_t> delta_state;

        for(uint_t  state_idx=0; state_idx < states.size(); ++state_idx ){

            real_t weighted_reward = 0.0;
            const State& state = states[state_idx];

            for(uint_t  action_idx=0; action_idx < ACTIONS_SIZE; ++action_idx){

                real_t final_pos = 0.0;
                real_t reward = 0.0;

                weighted_reward += (1.0/ACTIONS_SIZE)*(reward + (gamma_*value_map_(final_pos[0], final_pos[1])));

            }

            delta_state.push_back(std::abs(copy_value_map(state.i, state.j) - weighted_reward));
            copy_value_map(state.i, state.j) = weighted_reward;
        }

    value_map_ = copy_value_map;
    deltas.push_back(delta_state);
    }
}


}

int main(int argc, char** argv) {

    using namespace exe;


    
   
    return 0;
}

