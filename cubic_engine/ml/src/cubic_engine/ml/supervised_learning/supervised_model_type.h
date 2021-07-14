#ifndef SUPERVISED_MODEL_TYPE_H
#define SUPERVISED_MODEL_TYPE_H

#include "cubic_engine/base/cubic_engine_types.h"

namespace cengine {
namespace ml {

enum class SupervisedModelType{REGRESSOR, CLASSIFIER};

template<SupervisedModelType tp>
struct SupervisedModelTypeVal;

template<>
struct SupervisedModelTypeVal<SupervisedModelType::REGRESSOR>{

    typedef real_t value_t ;
};

template<>
struct SupervisedModelTypeVal<SupervisedModelType::CLASSIFIER>{

    typedef uint_t value_t ;
};

}

}

#endif // SUPERVISED_MODEL_TYPE_H
