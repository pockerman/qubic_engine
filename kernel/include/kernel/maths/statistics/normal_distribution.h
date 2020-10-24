#ifndef NORMAL_DISTRIBUTION_H
#define NORMAL_DISTRIBUTION_H

#include "kernel/base/types.h"
#include "kernel/maths/constants.h"
#include "kernel/utilities/common_uitls.h"
#include <random>
#include <cmath>

namespace kernel {
namespace maths {
namespace stats {

///
/// \brief The class NormalDistribution. Wrapper
/// class to std::normal_distribution
///
template<typename T>
class NormalDistribution
{

public:

    typedef typename std::normal_distribution<T>::result_type result_t;

    ///
    /// \brief NormalDistribution Constructor
    ///
    NormalDistribution(real_t mu=0.0, real_t std=1.0);

    template<typename Generator>
    result_t operator()(Generator& g){return dist_(g);}

    ///
    /// \brief pdf Calculate the probability of point x
    ///
    real_t pdf(T x)const;

    ///
    /// \brief mean Return the mean
    ///
    real_t mean()const{return dist_.mean();}

    ///
    /// \brief stddev. Return the standard deviation
    ///
    real_t stddev()const{return dist_.stddev();}

    ///
    /// \brief sample. Sample a value from the normal distribution
    ///
    result_t sample()const;

private:


    ///
    /// \brief dist_
    ///
    std::normal_distribution<T> dist_;

};

template<typename T>
NormalDistribution<T>::NormalDistribution(real_t mu, real_t std)
    :
      dist_(mu, std)
{}


template<typename T>
real_t
NormalDistribution<T>::pdf(T x)const{

    auto mu = mean();
    auto std = stddev();

    auto factor = 1.0/(std * std::sqrt(2.0*MathConsts::PI));
    auto x_minus_mu_sqr = utils::sqr((x-mu)/std);
    return factor*std::exp(-0.5*x_minus_mu_sqr);
}

template<typename T>
typename NormalDistribution<T>::result_t;
NormalDistribution<T>::sample()const{

    std::random_device rd{};
    std::mt19937 gen{rd()};
    return dist_(gen);

}

}

}

}

#endif // NORMAL_DISTRIBUTION_H
