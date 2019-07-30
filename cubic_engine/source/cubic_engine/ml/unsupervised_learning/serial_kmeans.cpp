#include "parml/unsupervised_learning/serial_kmeans.h"

namespace parml
{

template<typename DataPoint>
KMeans<parframepp::platform_type::Type::SERIAL,DataPoint>::KMeans(const KMeansControl& cntrl)
:
KMeansBase<DataPoint>(cntrl)
{}


template class KMeans<parframepp::platform_type::Type::SERIAL,ArmaVecDataSet<real_type> >;
template class KMeans<parframepp::platform_type::Type::SERIAL,ArmaVecDataSet<size_type> >;
template class KMeans<parframepp::platform_type::Type::SERIAL,ArmaVecDataSet<int> >;
template class KMeans<parframepp::platform_type::Type::SERIAL,ArmaVecDataSet<float> >;
    
}
