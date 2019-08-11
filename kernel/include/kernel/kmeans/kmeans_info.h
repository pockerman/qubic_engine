/* 
 * File:   kmeans_info.h
 * Author: david
 *
 * Created on October 24, 2016, 9:07 AM
 */

#ifndef KMEANS_INFO_H
#define	KMEANS_INFO_H

#include "base/types.h" //application types 
#include "base/algorithm_info.h"
#include <vector>

namespace kmeans
{
    
	/*
	 * @brief Small struct that wraps information about
	 * K-means to be passed to the client code upon completion
	 * of the algorithm
	 */
	struct KMeansInfo: public kernel::AlgInfo
	{
		  
		  /**
		   * @brief Array of the clusters along with the number of datapoints
		   * each cluster contains
		   */
		  std::vector<std::pair<uint_t, uint_t> > clusters;
		  
		  /**
		   * @brief Number of points for clustering
		   */
		  uint_t n_clustering_points;
		  
		  /**
		   * @brief Constructor
		   */
		  KMeansInfo();
		  
		  /**
		   * @brief Print the information related to the application
		   * of K-means algorithm to the given stream.
		   */
		  virtual std::ostream& print(std::ostream& out)const;   
	};

	inline
	std::ostream& 
	operator<<(std::ostream& out,const KMeansInfo& info )
	{
		  return info.print(out);
	}
    
}

#endif	/* KMEANS_INFO_H */

