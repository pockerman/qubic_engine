#ifndef KMEANS_INFO_H
#define	KMEANS_INFO_H

#include "kernel/utilities/algorithm_info.h"
#include "cubic_engine/base/cubic_engine_types.h"

#include <ostream>
#include <vector>

namespace cengine
{
    

/// \brief Small struct that wraps information about
/// K-means to be passed to the client code upon completion
/// of the algorithm
struct KMeansInfo: public kernel::AlgInfo
{
    
    /// \brief Array of the clusters along with the number of datapoints
    /// each cluster contains
    std::vector<std::pair<size_type,size_type> > clusters;
    
    /// \brief Number of points for clustering
    uint_t n_clustering_points;
    
    /// \brief Constructor
    KMeansInfo();
    
    /// \brief Print the information related to the application
    /// of K-means algorithm to the given stream.
    virtual std::ostream& print(std::ostream& out)const;   
};

inline
std::ostream& 
operator<<(std::ostream& out, const KMeansInfo& info )
{
    return info.print(out);
}

    
    
}



#endif	/* KMEANS_INFO_H */

