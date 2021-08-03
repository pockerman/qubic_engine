#ifndef ARRAY_UTILS_H
#define ARRAY_UTILS_H

#include "kernel/base/types.h"
#include <vector>
#include <algorithm> // for std::max/min_element
#include <iterator> // for std::distance
namespace kernel {
namespace utils{

///
/// \brief Returns the index of the element that has the maximum
/// value in the array. Implementation taken from
/// http://www.jclay.host/dev-journal/simple_cpp_argmax_argmin.html
///
template <typename T, typename A>
uint_t arg_max(const std::vector<T, A>& vec) {
  return static_cast<int>(std::distance(vec.begin(), max_element(vec.begin(), vec.end())));
}


///
/// \brief Returns the index of the element that has the minimum
/// value in the array. Implementation taken from
/// http://www.jclay.host/dev-journal/simple_cpp_argmax_argmin.html
///
template <typename T, typename A>
uint_t arg_min(const std::vector<T, A>& vec) {
  return static_cast<uint_t>(std::distance(vec.begin(), min_element(vec.begin(), vec.end())));
}

template<typename T>
std::vector<uint_t> max_indices(const DynVec<T>& vec){

    // find max value
    auto max_val = blaze::max(vec);

    auto result = std::vector<uint_t>();
    auto counter = 0;

    for(uint_t i=0; i<vec.size(); ++i){
        T value = vec[i];
        if(value == max_val){
            result.push_back(i);
        }
    }

    return result;
}

///
/// \brief Returns the indices of vec
/// where the maximum value in vec occurs
///
template<typename VecTp>
std::vector<uint_t> max_indices(const VecTp& vec){

    // find max value
    auto max_val = std::max_element(vec.begin(), vec.end());

    auto result = std::vector<uint_t>();
    auto counter = 0;

    std::for_each(vec.begin(), vec.end(),
                  [&](auto val){
        if(val == max_val){
            result.push_back(counter);
        }
        ++counter;
    });

    return result;
}




}

}

#endif // ARRAY_UTILS_H
