#ifndef ITERATIVE_ALGORITHM_RESULT_H
#define ITERATIVE_ALGORITHM_RESULT_H

#include "kernel/base/types.h"
#include <ostream>
#include <chrono>

namespace kernel{

///
/// \brief The IterativeAlgorithmResult struct. Helper
/// struct to assemble the result of an iterative algorithm
///
struct IterativeAlgorithmResult  {

    bool converged=false;
    real_t tolerance;
    real_t residual;
    uint_t num_iterations;
    std::chrono::seconds total_time;

    virtual std::ostream& print(std::ostream& out)const;
};


inline
std::ostream& operator<<(std::ostream& out, const IterativeAlgorithmResult& result){
    return result.print(out);
}
}
#endif // ITERATIVE_ALGORITHM_RESULT_H
