#include "kernel/base/config.h"

#ifdef USE_OPENMP
#include "kernel/parallel/threading/openmp_executor.h"

namespace kernel
{
uint_t
OMPExecutor::get_n_threads()const{
    uint_t n_threads = omp_get_num_threads();

    // this means that we are outside a parallel region
    if(n_threads == 1){
        n_threads = n_threads_;
    }

    return n_threads;
}
}

#endif
