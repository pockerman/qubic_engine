#ifndef ITERATIVE_ALGORITHM_CONTROLLER_H
#define ITERATIVE_ALGORITHM_CONTROLLER_H

#include "kernel/base/types.h"
#include "kernel/utilities/iterative_algorithm_result.h"
#include <limits>


namespace kernel
{

class IterativeAlgorithmController
{
public:

   /**
    * Constructor
    */
   IterativeAlgorithmController(uint_t maxIterations, real_t exitTolerance);

   /**
    * Returns true if the iterations of the algorithm should be continued
    */

    bool continue_iterations();

    /**
      * Returns the current iteration index
    */
    uint_t get_current_tteration()const{
           return current_iteration_idx_;
    }

    /**
     * Returns the exit tolerance for the algorithm
    */
    real_t get_exit_tolerance()const{
        return exit_tolerance_;
    }

    /**
     * Returns the state of the controller
     */
    IterativeAlgorithmResult get_state()const;

    /**
     * Update the residual
     */
    void updateResidual(real_t res){

      current_res_ = res;
    }

private:


    uint_t max_iterations_;
    real_t exit_tolerance_;
    uint_t current_iteration_idx_;
    real_t current_res_;
};

inline
IterativeAlgorithmController::IterativeAlgorithmController(uint_t max_iterations, real_t exit_tolerance)
    :
  max_iterations_(max_iterations),
  exit_tolerance_(exit_tolerance),
  current_iteration_idx_(0),
  current_res_(std::numeric_limits<real_t>::max())
{}

}

#endif // ITERATIVE_ALGORITHM_CONTROLLER_H
