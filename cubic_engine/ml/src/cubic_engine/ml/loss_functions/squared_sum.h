#ifndef SQUARED_SUM_H
#define SQUARED_SUM_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/loss_functions/accumulator_types.h"

namespace cengine {
namespace ml {

template<typename AccumTp>
struct SqrAccumulator;



template<>
struct SqrAccumulator<SumT>
{
    template<typename VecTp>
    real_t operator()(const VecTp& v1, const VecTp& v2)const;
};

template<typename VecTp>
real_t
SqrAccumulator<SumT>::operator()(const VecTp& v1, const VecTp& v2)const{

    real_t result = 0.0;
    for(uint_t i=0; i<v1.size(); ++i){
        result += (v1[i] + v2[i])*(v1[i] + v2[i]);
    }

    return result;
}

template<>
struct SqrAccumulator<MinusT>
{
    template<typename VecTp>
    real_t operator()(const VecTp& v1, const VecTp& v2)const;
};

template<typename VecTp>
real_t
SqrAccumulator<MinusT>::operator()(const VecTp& v1, const VecTp& v2)const{

    real_t result = 0.0;
    for(uint_t i=0; i<v1.size(); ++i){
        result += (v1[i] - v2[i])*(v1[i] - v2[i]);
    }

    return result;
}


}

}

#endif // SQUARED_SUM_H
