#include "cubic_engine/rl/algorithms/algorithm_base.h"

namespace cengine{
namespace rl {
namespace algos {

AlgorithmBase::AlgorithmBase(uint_t n_max_itrs, real_t tolerance)
    :
    itr_ctrl_(n_max_itrs, tolerance)
{}


kernel::IterativeAlgorithmResult
AlgorithmBase::train(){

    this->actions_before_training_iterations();

    while(itr_ctrl_.continue_iterations()){
        this->step();
    }

    this->actions_after_training_iterations();

    return itr_ctrl_.get_state();

}

}

}
}
