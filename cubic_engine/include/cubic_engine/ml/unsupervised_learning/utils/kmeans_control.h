#ifndef KMEANS_CONTROL_H
#define	KMEANS_CONTROL_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/iterative_algorithm_controller.h"
#include "kernel/base/kernel_consts.h"

#include <ostream>
namespace cengine
{

/**
 * @brief Small struct that wraps 
 * configuration parameters for k-means algorithm
 */    

struct KMeansControl: public kernel::IterativeAlgorithmController
{

    /// \brief Number of clusters
    uint_t k;
    
    /// \brief Flag indicating whether a random restart should
    /// be performed when an empty cluster is located
    bool random_restart_on_empty_cluster;
    
    /// \brief Flag indicating whether the algorithm should
    /// continue its execution when an empty cluster is detected
    bool continue_on_empty_cluster;
    

    /// \brief Constructor
    KMeansControl(uint_t k_, uint_t itrs=1000);
              
};

inline
KMeansControl::KMeansControl(uint_t k_, uint_t itrs)
            :
            kernel::IterativeAlgorithmController(itrs, kernel::KernelConsts::tolerance()),
            k(k_),
            random_restart_on_empty_cluster(true),
            continue_on_empty_cluster(false)
{}

}



#endif	/* KMEANS_CONTROL_H */

