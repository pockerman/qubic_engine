#include "parframe/base/types.h"

#define BOOST_TEST_MODULE KMEANS_TEST_CLUSTER_WITH_INITIALIZATION

//unit testing from boost
#include <boost/test/unit_test.hpp>
#include <vector>
#include <utility>


namespace testspace
{
    
}

BOOST_AUTO_TEST_CASE (KMEANS_CONFIG_ZERO_NUMBER_CLUSTERS_TEST)
{
		/*
			Test the scenario where the application attempts to use the 
			K-means algorithm without specifying the number of clusters
		*/

        /*using namespace kernel;

		// configuration for K-means
    kmeans::KMeans::config_t config;

		// create the algorithm
		kmeans::KMeans algo(config);

		// empty initialization list
		std::vector<std::pair<uint_t, Mat<real_type>::point_t> > init;

		// create the assumed dataset
		Mat<real_type> dataset;

		// create the similarity measure
		EuclideanDist similarity;

    //we should get an std::invalid_argument
    BOOST_CHECK_THROW(algo.cluster(dataset, similarity, init), std::invalid_argument);*/
    
}


BOOST_AUTO_TEST_CASE (KMEANS_NUMBER_CENTROIDS_NOT_EQUAL_TO_NUMBER_CLUSTERS_TEST)
{
		/*
			Test the scenario where the application attempts to use the 
			K-means algorithm but the supplied initial number of clusters is different than 
			what is specified in the alforithm configuration
		*/

    /*using namespace kernel;

		// configuration for K-means
    kmeans::KMeans::config_t config;
		config.n_clusters = 2;

		// create the algorithm
		kmeans::KMeans algo(config);

		// empty initialization list
		std::vector<std::pair<uint_t, Mat<real_type>::point_t> > init(3);

		// create the assumed dataset
		Mat<real_type> dataset;

		// create the similarity measure
		EuclideanDist similarity;

    //we should get an std::invalid_argument
    BOOST_CHECK_THROW(algo.cluster(dataset, similarity, init), std::invalid_argument);
    */
}
