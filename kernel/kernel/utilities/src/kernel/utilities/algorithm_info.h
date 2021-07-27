#ifndef ALGORITHM_INFO_H
#define	ALGORITHM_INFO_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"

#include <chrono>
#include <ostream>
#include <limits>

namespace kernel
{
 
///
/// \brief A small struct that holds basic information
///about the performance of an algorithm.
///
struct AlgInfo
{
    ///
    /// \brief How many iterations the algorithm performed
    ///
    uint_t niterations;

    ///
    /// \brief The residual after the algorithm terminated
    ///
    real_t residual;

    ///
    /// \brief Flag indicating if the algorithm converged
    ///
    bool converged;

    ///
    /// \brief The exit tolerace used
    ///
    real_t tolerance;

    ///
    /// \brief The total time duration of the algorithm
    ///
    std::chrono::duration<real_t> runtime;

     ///
     /// \brief How many threads the algorithm used
     ///
     uint_t nthreads;

     ///
     /// \brief How many processors the algorithm used
     ///
     uint_t nprocs;

     ///
     /// \brief Constructor
     ///
     AlgInfo()
              :
             niterations(KernelConsts::invalid_size_type()),
             residual(std::numeric_limits<real_t>::max()),
             converged(false),
             runtime(),
             nthreads(1),
             nprocs(1)
      {}

      ///
      /// \brief Print the information about the performance of
      /// the algorithm on the given stream
      ///
      std::ostream& print(std::ostream& out)const;
};

inline
std::ostream& operator<<(std::ostream& out,const AlgInfo& info ){
    return info.print(out);
}
		    
}

#endif	/* ALGORITHM_INFO_H */

