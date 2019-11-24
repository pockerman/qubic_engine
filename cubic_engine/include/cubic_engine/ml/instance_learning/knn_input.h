#ifndef KNN_INPUT_H
#define KNN_INPUT_H

#include "cubic_engine/base/cubic_engine_types.h"

namespace cengine
{

struct KnnInput
{
    /**
     * @brief k: The number of neighbors to consider
     */
    uint_t k;
};

}

#endif // KNN_INPUT_H
