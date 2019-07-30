/* 
 * File:   kmeans_control.h
 * Author: david
 *
 * Created on October 24, 2016, 9:16 AM
 */

#ifndef KMEANS_CONTROL_H
#define	KMEANS_CONTROL_H

#include "parml/base/parml.h"
#include "parml/base/algorithm_control.h"

namespace parml
{

/**
 * @brief Small struct that wraps 
 * configuration parameters for k-means algorithm
 */    

struct KMeansControl:public AlgControl
{
    /**
     * @brief Number of clusters
     */
    size_type k;
    
    /**
     * @brief Flag indicating whether a random restart should
     * be performed when an empty cluster is located
     */
    bool random_restart_on_empty_cluster;
    
    /**
     * @brief Flag indicating whether the algorithm should
     * continue its execution when an empty cluster is detected 
     */
    bool continue_on_empty_cluster;
    
    /**
     * @brief The number of features
     */
    size_type n_features;
    
    /**
     * @brief The number of data points
     */
    size_type n_training_examples;
    
    /**
     * @brief Constructor
     */
    KMeansControl(size_type k_,size_type itrs=1000);
           
    
     /**
     * @brief print the control data to the given stream
     */
    virtual std::ostream& print(std::ostream& out)const;
    
};

//template and inline methods
inline
KMeansControl::KMeansControl(size_type k_,size_type itrs)
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

