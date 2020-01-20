#ifndef KMEANS_CONTROL_H
#define	KMEANS_CONTROL_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/iterative_algorithm_controller.h"

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
    
    /// \brief The number of features
    uint_t n_features;
    
    /// \brief The number of data points
    uint_t n_training_examples;
     
    /// \brief Constructor
    KMeansControl(uint_t k_, uint_t itrs=1000);
           
    /// \brief print the control data to the given stream
    virtual std::ostream& print(std::ostream& out)const;
    
};

//template and inline methods
inline
KMeansControl::KMeansControl(uint_t k_,uint_t itrs)
            :
            AlgControl(itrs,ParML::tolerance()),
            k(k_),
            random_restart_on_empty_cluster(true),
            continue_on_empty_cluster(false),
            n_features(ParML::iuint()),
            n_training_examples(ParML::iuint())
{}

inline
std::ostream& operator<<(std::ostream& out,const KMeansControl& control ){
    return control.print(out);
}
 
    
    
    
}



#endif	/* KMEANS_CONTROL_H */

