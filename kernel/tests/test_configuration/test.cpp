#include "parframe/base/types.h"

#define BOOST_TEST_MODULE KMEANS_TEST_CONFIG

//unit testing from boost
#include <boost/test/unit_test.hpp>
#include <vector>
#include <utility>


namespace testspace
{
    
}

BOOST_AUTO_TEST_CASE (KMEANS_CONFIG_CONSTRUCTOR_TEST)
{
    //kmeans::KMeans::config_t config;

    //we should get an std::logic_error
    //BOOST_CHECK_THROW(tac.load(test_file,emptypath),std::logic_error);
    
    //boost::filesystem::path test_path(PHYSIM_TEST_DATA_DIR);
    
    //this should throw as this file name does not exist
    //BOOST_CHECK_THROW(tac.load(test_file,test_path),std::logic_error);
    
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

		// create the assumed dataset
		Mat<real_type> dataset;

		// empty initialization list
		std::vector<std::pair<uint_t, Mat<real_type>::point_t> > init;

		// create the similarity measure
		EuclideanDist similarity;

    //we should get an std::invalid_argument
    BOOST_CHECK_THROW(algo.cluster(dataset, similarity, init), std::invalid_argument);*/
    
}
