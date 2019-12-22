#include "kernel/utilities/iterative_algorithm_controller.h"

namespace kernel
{

IterativeAlgorithmResult
IterativeAlgorithmController::get_state()const{

        IterativeAlgorithmResult result;
        result.converged = current_res_ <= exit_tolerance_ ? true:false;
        result.num_tterations = current_iteration_idx_;
        result.tolerance = current_res_;
        return result;
}

bool
IterativeAlgorithmController::continue_iterations(){

  if(current_iteration_idx_ >= max_iterations_){
        return false;
  }
  else if(current_res_ < exit_tolerance_){

        current_iteration_idx_++;
        return false;
  }

  current_iteration_idx_++;
  return true;
}
}
