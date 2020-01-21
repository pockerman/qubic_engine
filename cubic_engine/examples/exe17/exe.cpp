
#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/unsupervised_learning/serial_kmeans.h"
#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_control.h"
#include "cubic_engine/ml/unsupervised_learning/utils/cluster.h"

#include "kernel/utilities/data_set_loaders.h"
#include "kernel/maths/lp_metric.h"
#include "kernel/maths/matrix_utilities.h"

#include <iostream>

int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using cengine::Cluster;


    try{

        typedef DynVec<real_t> point_t;
        DynMat<real_t> data;

        cengine::KMeansControl control(2);
        cengine::KMeans<Cluster<point_t>> kmeans(control);

        // the metric we use
        kernel::LpMetric<2> l2_norm;

        auto init = [&](const DynMat<real_t>& data, uint_t k, std::vector<point_t>& centroids ){
            kernel::extract_randomly(data, centroids, k, false);
        };

        auto result = kmeans.cluster(data, l2_norm, init);
        std::cout<<result<<std::endl;


    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}

