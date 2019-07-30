#include "parml/unsupervised_learning/cput_c11_kmeans.h"

namespace parml
{
 
template<typename DataPoint>
KMeans<parframepp::platform_type::Type::CPUT_C11,DataPoint>::KMeans(const KMeansControl& cntrl)
:
KMeansBase<DataPoint>(cntrl),
parframepp::par_indexed_object()
{}

namespace kmeans_detail
{
    
Iteration<parframepp::platform_type::Type::CPUT_C11>::Iteration(const KMeansControl& cntrl_)
:
cntrl(cntrl_),
parframepp::model_impl_base(),
main_task_(nullptr)
{}
    
    
}


template class KMeans<parframepp::platform_type::Type::CPUT_C11,ArmaVecDataSet<real_type> >;
template class KMeans<parframepp::platform_type::Type::CPUT_C11,ArmaVecDataSet<size_type> >;
template class KMeans<parframepp::platform_type::Type::CPUT_C11,ArmaVecDataSet<int> >;
template class KMeans<parframepp::platform_type::Type::CPUT_C11,ArmaVecDataSet<float> >;
    
    
}
