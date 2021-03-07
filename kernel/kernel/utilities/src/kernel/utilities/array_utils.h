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

}

}

#endif // ARRAY_UTILS_H
