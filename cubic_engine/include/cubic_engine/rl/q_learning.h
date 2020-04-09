#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/base/kernel_consts.h"

#include <boost/noncopyable.hpp>
#include <string>
#include <map>
#include <random>

namespace cengine {
namespace rl{

struct QLearningInput
{
    real_t learning_rate;
    real_t exploration_factor;
    real_t discount_factor;
    bool use_exploration;
};


/// \brief Tabular implementation of Q-learning algorithm
template<typename WorldTp>
class QTableLearning: private boost::noncopyable
{

public:

    /// \brief The type of the world
    typedef WorldTp world_t;

    /// \brief Constructor
    QTableLearning(QLearningInput&& input);

    /// \brief Train on the given world
    void train(world_t& world );

private:

    /// \brief Basic input
    QLearningInput input_;

    /// \brief The QTable. A state is identified
    /// by an id. At each state a number of actions are
    /// possible. The score for each action is stored in
    /// a contiguous array
    std::map<uint_t, std::vector<real_t>> qtable_;

    /// \brief The world used by the agent
    world_t* world_ptr_;

    ///Initialize the tabular implementation
    void initialize_(world_t& world);

    /// \brief Initialize the QTable entries
    /// for the given state
    template<typename StateTp>
    void initialize_state_(const StateTp& state);

    /// \brief Returns the action idx for
    /// the given state idx that has the maximum score
    uint_t get_maximum_action_for_state_(uint_t state_idx)const;

};

template<typename WorldTp>
QTableLearning<WorldTp>::QTableLearning(QLearningInput&& input)
    :
    input_(std::move(input)),
    qtable_(),
    world_ptr_(nullptr)
{}

template<typename WorldTp>
void
QTableLearning<WorldTp>::initialize_(typename QTableLearning<WorldTp>::world_t& world){


    /// loop over the states of the world and initialize
    /// the action scores
    for(uint_t s=0; s<world.n_states(); ++s){
        initialize_state_(world.get_state_by_id(s));
    }

    /// finally set the world pointer
    world_ptr_ = &world;
}

template<typename WorldTp>
template<typename StateTp>
void
QTableLearning<WorldTp>::initialize_state_(const StateTp& state){
    qtable_.insert({state.get_id(), std::vector<real_t>(state.n_actions(), 0.0)});
}

template<typename WorldTp>
uint_t
QTableLearning<WorldTp>::get_maximum_action_for_state_(uint_t state_idx)const{

}

template<typename WorldTp>
void
QTableLearning<WorldTp>::train(world_t& world ){

    /// initialize the table
    initialize_(world);

    /// get the current state of the world
    auto state = world.get_current_state();

    auto idx = kernel::KernelConsts::invalid_size_type();

    /// generate a random number
    /// between 0 and 1
    auto r = 0.0;
    if(input_.use_exploration){

        ///Will be used to obtain a seed for the random number engine
        std::random_device rd;

        ///Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.1);
        r = dis(rd);
    }

    /// replace with random action to
    /// promote exploration
    if(input_.use_exploration && r < input_.exploration_factor){

        ///Will be used to obtain a seed for the random number engine
        std::random_device rd;

        ///Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, state.n_actions());
        idx = dis(rd);

    }
    else{

        /// select the action with the maximum
        /// score for this state
       idx = get_maximum_action_for_state_(state.get_id());
    }

    /// in the current state execute the
    /// action
    world.execute_action(idx);



}




}

}

#endif // Q_LEARNING_H
