/* 
 * File:   algorithm_info.h
 * Author: david
 *
 * Created on July 28, 2016, 9:49 AM
 */

#ifndef ALGORITHM_INFO_H
#define	ALGORITHM_INFO_H

#include "parframe/base/types.h" //application types
#include "parframe/base/kernel_consts.h"

#include <chrono>
#include <ostream>
#include <limits>

namespace parframe
{
 
	/**
	 * @brief A small struct that holds basic information
	 * about the performance of an algorithm.
	 */
	struct AlgInfo
	{
		  /**
		   *@brief How many iterations the algorithm performed
		   */ 
		  uint_t niterations;
		  
		  /**
		   * @brief The residual after the algorithm terminated
		   */
          real_t residual;
		  
		  /**
		   * @brief Flag indicating if the algorithm converged
		   */
		  bool converged;
		  
		  /**
		   * @brief The total time duration of the algorithm
		   */
          std::chrono::duration<real_t> runtime;
		  
		  /**
		   * @brief How many threads the algorithm used
		   */
		  uint_t nthreads;
		  
		  /**
		   * @brief How many processors the algorithm used
		   */
		  uint_t nprocs; 
		  
		  /**
		   * @brief Constructor
		   */
		  AlgInfo()
		          :
		         niterations(kernel_consts::invalid_size_type()),
                 residual(std::numeric_limits<real_t>::max()),
		         converged(false),
		         runtime(),
		         nthreads(1),
		         nprocs(1)
		  {}
		  
		  
		  /**
		   * @brief Print the information about the performance of
		   * the algorithm on the given stream
		   */
		  virtual std::ostream& print(std::ostream& out)const;   
	};

	inline
	std::ostream& operator<<(std::ostream& out,const AlgInfo& info ){
		  
		  return info.print(out);
	}
		    
}

#endif	/* ALGORITHM_INFO_H */

