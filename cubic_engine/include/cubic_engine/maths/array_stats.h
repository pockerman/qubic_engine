#ifndef ARRAY_STATS_H
#define ARRAY_STATS_H

#include "cubic_engine/base/cubic_engine_types.h"

#include <stdexcept>
#include <cmath>
#include <future>
#include <thread>
#include <iostream>

/**
 * \brief Useful statistical computations on arrays
 */
namespace cengine
{

/**
  * Compute the sum of the array elements
  **/
template<typename ArrayType>
auto sum(const ArrayType& c){

    auto rslt = c[0];

    for(uint_t i =1; i<c.size(); ++i){
        rslt += c[i];
    }

    return rslt;
}

/**
  * Compute the sum of squares the array elements
  **/
template<typename ArrayType>
typename ArrayType::value_type
sum_sqr(const ArrayType& c){

    auto rslt = c[0]*c[0];

    for(uint_t i =1; i<c.size(); ++i){
        rslt += c[i]*c[i];
    }

    return rslt;
}


/**
  * Compute the arithmetic mean of the given array
  **/
template<typename ArrayType>
real_t mean(const ArrayType& c){
    return sum(c)/c.size();
}


/**
  * Compute the dot produce the two arrays
  **/
template<typename ArrayType>
typename ArrayType::value_type
dot_product(const ArrayType& c1, const ArrayType& c2){

    if(c1.size() != c2.size()){
        throw std::invalid_argument("Invalid arrays sizes: "+std::to_string(c1.size())+" not equal to: "+std::to_string(c2.size()));
    }

    auto rslt = c1[0]*c2[0];

    for(uint_t i =1; i<c1.size(); ++i){
        rslt += c1[i]*c2[i];
    }

    return rslt;
}

/**
  * Compute the Pearson correlation coefficient between the two arrays
  * see https://en.wikipedia.org/wiki/Pearson_correlation_coefficient
  **/

template<typename ArrayType>
real_t pearson_corr(const ArrayType& c1, const ArrayType& c2){

    if(c1.size() != c2.size()){
        throw std::invalid_argument("Invalid arrays sizes: "+std::to_string(c1.size())+" not equal to: "+std::to_string(c2.size()));
    }

    auto mc1 = std::async(std::launch::async, cengine::mean<ArrayType>, c1);
    auto mc2 = std::async(std::launch::async, cengine::mean<ArrayType>, c2);
    auto sqr_sum1 = std::async(std::launch::async, cengine::sum_sqr<ArrayType>, c1);
    auto sqr_sum2 = std::async(std::launch::async, cengine::sum_sqr<ArrayType>, c2);
    auto dot_product_op = std::async(std::launch::async, cengine::dot_product<ArrayType>, c1, c2);

    auto mu1_rslt = mc1.get();
    auto mu2_rslt = mc2.get();
    auto dot_product_rslt = dot_product_op.get();
    auto sqr_sum1_rslt = sqr_sum1.get();
    auto sqr_sum2_rslt = sqr_sum2.get();
    return (dot_product_rslt - c1.size()*mu1_rslt*mu2_rslt)/(std::sqrt(sqr_sum1_rslt - c1.size()*mu1_rslt*mu1_rslt)*std::sqrt(sqr_sum2_rslt - c1.size()*mu2_rslt*mu2_rslt));
}

}

#endif // ARRAY_STATS_H
