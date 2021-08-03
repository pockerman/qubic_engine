#include "cubic_engine/rl/algorithms/algorithm_base.h"
#include "kernel/utilities/iterative_algorithm_controller.h"
//#include <iostream>

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
        //std::cout<<"Iteration="<<itr_ctrl_.get_current_iteration()<<" of "<<itr_ctrl_.get_max_iterations()<<std::endl;
        this->step();
    }

    this->actions_after_training_iterations();

    return itr_ctrl_.get_state();

}

void
AlgorithmBase::reset(){
    itr_ctrl_.reset();
}

}

}
}
