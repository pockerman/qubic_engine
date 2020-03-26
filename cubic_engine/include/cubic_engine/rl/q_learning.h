#ifndef Q_LEARNING_H
#define Q_LEARNING_H
#include "cubic_engine/base/cubic_engine_types.h"

namespace cengine {
namespace rl{

struct QLearningInput
{
    real_t learning_rate;
    real_t exploration_factor;
    real_t discount_factor;
};


/// \brief Tabular implementation of Q-learning algorithm
class QLearning
{

public:

    /// \brief Constructor
    QLearning(QLearningInput&& input);

private:


};

}

}

#endif // Q_LEARNING_H
