#ifndef KNN_CONTROL_H
#define KNN_CONTROL_H

#include "cubic_engine/base/cubic_engine_types.h"

namespace cengine{
namespace ml{

struct KnnControl
{
    /// \brief k: The number of neighbors to consider
    uint_t k;

    /// \brief Constructor
    KnnControl(uint_t k_)
        :
        k(k_)
    {}

};

}
}
#endif // KNN_INPUT_H
