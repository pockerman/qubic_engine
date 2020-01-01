#ifndef ITERATIVE_ALGORITHM_RESULT_H
#define ITERATIVE_ALGORITHM_RESULT_H

#include "kernel/base/types.h"

namespace kernel
{

struct IterativeAlgorithmResult  {

    bool converged=false;
    real_t tolerance;
    real_t residual;
    uint_t num_iterations;
};
}
#endif // ITERATIVE_ALGORITHM_RESULT_H
