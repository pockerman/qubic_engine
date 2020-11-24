#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/unsupervised_learning/serial_kmeans.h"
#include "cubic_engine/ml/unsupervised_learning/utils/cluster.h"
#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_control.h"
#include "kernel/utilities/data_set_loaders.h"
#include "kernel/maths/matrix_traits.h"
#include "kernel/data_structs/data_set_wrapper.hpp"
#include "kernel/maths/lp_metric.h"
#include "kernel/maths/matrix_utilities.h"
#include "kernel/base/kernel_consts.h"

#include <iostream>
#include <map>
#include <vector>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::DynVec;
using cengine::ml::KMeans;

void compute_purity(){
	
}

std::map<uint_t, std::vector<uint_t>> 
get_classes(const DynVec<uint_t>& labels){
	
	std::map<uint_t, std::vector<uint_t>> clusters;
	
	for(uint_t i = 0; i<labels.size(); ++i){
		
		auto itr = clusters.find(labels[i]);
		
		if(itr != clusters.end()){
			itr->second.push_back(i);
		}
		else{
			std::vector<uint_t> indexes = {i};
			clusters.insert({labels[i], indexes});
		}
	}
	
	
	return clusters;
} 


}

int main() {
   
    using namespace example;
    
    try{
		
		
		auto data = kernel::load_iris_data_set(false);
		
		auto classes = get_classes(data.second);
		
		std::cout<<kernel::KernelConsts::info_str()<<"Number of classes="<<classes.size()<<std::endl;
		
		kernel::data_structs::DataSetWrapper<DynMat<real_t>> dataset;
		dataset.load_from(data.first);
		
        cengine::KMeansConfig config(classes.size(), 100);
		config.set_show_iterations_flag(true);
		KMeans<cengine::Cluster<DynVec<real_t>>> clusterer(config);
        
		kernel::LpMetric<2> metric;
		
		typedef DynVec<real_t> point_t;
		auto init = [&](const kernel::data_structs::DataSetWrapper<DynMat<real_t>>& data, 
		                uint_t k, std::vector<point_t>& centroids ){
							
             kernel::extract_randomly(data.get_storage(), centroids, k, false);
        };

		auto out = clusterer.cluster(dataset, metric, init);
		std::cout<<out<<std::endl;
		
		// get the clusters 
		auto& clusters = clusterer.get_clusters();
		
		// compute the purity for each cluster.
		
    }
    catch(std::runtime_error& e){
        std::cerr<<"Runtime error: "
                 <<e.what()<<std::endl;
    }
    catch(std::logic_error& e){
        std::cerr<<"Logic error: "
                 <<e.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }
   
    return 0;
}

