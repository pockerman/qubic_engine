
#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/unsupervised_learning/serial_kmeans.h"
#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_control.h"
#include "cubic_engine/ml/unsupervised_learning/utils/cluster.h"

#include "kernel/utilities/data_set_loaders.h"
#include "kernel/maths/lp_metric.h"
#include "kernel/maths/matrix_utilities.h"

#include <random>
#include <iostream>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::DynVec;
using cengine::Cluster;

void simple_example(){

    typedef DynVec<real_t> point_t;
    DynMat<real_t> data = kernel::load_kmeans_test_data();

    cengine::KMeansConfig control(2);
    control.set_show_iterations_flag(true);

    cengine::KMeans<Cluster<point_t>> kmeans(control);

    // the metric we use
    kernel::LpMetric<2> l2_norm;

    auto init = [&](const DynMat<real_t>& data, uint_t k, std::vector<point_t>& centroids ){
        kernel::extract_randomly(data, centroids, k, false);
    };

    auto result = kmeans.cluster(data, l2_norm, init);
    std::cout<<result<<std::endl;

    const auto& clusters = kmeans.get_clusters();

    for(auto& cluster : clusters){
        std::cout<<"Cluster id...."<<cluster.id<<std::endl;
        std::cout<<"Cluster size.."<<cluster.points.size()<<std::endl;

        for(uint_t p=0; p<cluster.points.size(); ++p){
            const auto point = kernel::get_row(data, cluster.points[p]);
            std::cout<<point[0]<<","<<point[1]<<std::endl;
        }
    }

    // let's save the clusters for visualization
    kmeans.save("kmeans_simple_example.csv", data);
}

void normal_example(){

    typedef DynVec<real_t> point_t;
    DynMat<real_t> data(1000, 2);

    std::random_device rd;
    std::mt19937 gen(rd());

    // mean = 0, std = 1.0
    std::normal_distribution<real_t> distribution;

    for(uint_t r=0; r<data.rows(); ++r){

        for(uint_t c=0; c<data.columns(); ++c){
            real_t x = distribution(gen);

            data(r,c) = x;
        }
    }


    cengine::KMeansConfig control(3);
    control.set_show_iterations_flag(true);

    cengine::KMeans<Cluster<point_t>> kmeans(control);

    // the metric we use
    kernel::LpMetric<2> l2_norm;

    auto init = [&](const DynMat<real_t>& data, uint_t k, std::vector<point_t>& centroids ){
        kernel::extract_randomly(data, centroids, k, false);
    };

    auto result = kmeans.cluster(data, l2_norm, init);
    std::cout<<result<<std::endl;

    const auto& clusters = kmeans.get_clusters();

    for(auto& cluster : clusters){
        std::cout<<"Cluster id...."<<cluster.id<<std::endl;
        std::cout<<"Cluster size.."<<cluster.points.size()<<std::endl;
    }

    // let's save the clusters for visualization
    kmeans.save("kmeans_normal_example.csv", data);
}


}

int main(){


    try{

        example::simple_example();
        example::normal_example();

    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}

