#ifndef ITERATION_CONTROL_H
#define ITERATION_CONTROL_H

#include "parframe/base/types.h"

namespace parframe
{

/**
 * @brief A small struct that wraps the basic
 * setup of an iterative algorithm.
 */

struct ItrCtrl
{

    /// \brief Maximum number of iterations to perform
    uint_t max_num_itrs;

    /// \brief Maximum number of threads to use
    uint_t max_n_threads;

    /// \brief Tolerance
    real_t tolerance;


    ItrCtrl(uint_t max_num_itrs_, uint_t max_n_threads_, real_t tolerance_)
        :
     max_num_itrs(max_num_itrs_),
     max_n_threads(max_n_threads_),
     tolerance(tolerance_)
    {}


};

}

#endif // ITERATION_CONTROL_H
