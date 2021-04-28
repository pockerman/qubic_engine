#ifndef RL_ITERATIVE_ALGO_INPUT_H
#define RL_ITERATIVE_ALGO_INPUT_H

#include "cubic_engine/base/cubic_engine_types.h"

namespace cengine {
namespace rl {

///
/// \brief The RLIterativeAlgoInput struct
/// Helper struct that assembles the input for the RL algorithms classes.
///
struct RLIterativeAlgoInput
{
    ///
    /// \breif learning_rate The learning rate
    ///
    real_t learning_rate{0.01};

    ///
    /// \brief epsilon. The exploration coefficient
    ///
    real_t epsilon{1.0};

    ///
    /// \brief discount_factor. The gamma coefficient
    ///
    real_t discount_factor{0.6};

    ///
    /// \brief show_iterations. Flag indicating if informative
    /// messages be printed during training
    ///
    bool show_iterations{true};

    ///
    /// \brief max_num_iterations. How many iterations should be
    /// performed per training episode.
    ///
    uint_t max_num_iterations;

    ///
    /// \brief total_episodes. Total number of training episodes
    ///
    uint_t total_episodes;

    ///
    /// \brief random_seed
    ///
    uint_t random_seed;
};

}

}

#endif // RL_ITERATIVE_ALGO_INPUT_H
