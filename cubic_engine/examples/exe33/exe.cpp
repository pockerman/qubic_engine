#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/unsupervised_learning/bisection_k_means.h"
#include "cubic_engine/include/cubic_engine/ml/unsupervised_learning/utils/cluster.h"
#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_control.h"
#include "kernel/utilities/data_set_loaders.h"
#include "kernel/maths/matrix_traits.h"
#include "kernel/include/kernel/data_structs/data_set_wrapper.hpp"
#include "kernel/maths/lp_metric.h"

#include <iostream>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::DynVec;
using cengine::ml::BisectionKMeans;

}

int main() {
   
    using namespace example;
    
    try{
		
		auto dataloader = [](DynMat<real_t>& set){
			kernel::load_random_set_one(set);
		};
		kernel::data_structs::DataSetWrapper<DynMat<real_t>> dataset;
		
		dataset.load_from(dataloader);
		
        cengine::KMeansConfig config(5, 100);
		BisectionKMeans<cengine::Cluster<DynVec<real_t>>> clusterer(config);
        
		kernel::LpMetric<2> metric;
		clusterer.cluster(dataset, metric);
        
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

