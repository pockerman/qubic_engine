#ifndef ERROR_FUNCTION_FACTORY_H
#define ERROR_FUNCTION_FACTORY_H

#include "kernel/base/config.h"
#include "kernel/base/types.h"
#include "kernel/maths/functions/function_base.h"
#include "kernel/maths/errorfunctions/error_function_type.h"
#include "kernel/maths/errorfunctions/sse_function.h"
#include "kernel/maths/errorfunctions/mse_function.h"
#include "kernel/parallel/utilities/result_holder.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

#include <memory>


namespace kernel
{


///
/// \brief The ErrFuncFactory class
///
class ErrFuncFactory
{
  public:

    ///
    ///
    ///
    ErrFuncFactory()=default;

    ///
    ///
    ///
    template<typename HypothesisFn, typename DataSetType, typename LabelsType>
    std::shared_ptr<FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>> build(ErrorFuncType type, HypothesisFn& h);


    ///
    ///
    ///
    template<typename HypothesisFn, typename DataSetType,
             typename LabelsType, typename RegularizerFn>
    std::shared_ptr<FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>> build(ErrorFuncType type, HypothesisFn& h, const RegularizerFn& r);


};

template<typename HypothesisFn, typename DataSetType, typename LabelsType>
std::shared_ptr<FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>>
ErrFuncFactory::build(ErrorFuncType type, HypothesisFn& h){

    auto ptr = std::shared_ptr<FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>>();
    switch(type){

     case ErrorFuncType::MSE:
    {
        ptr = std::make_shared<MSEFunction<HypothesisFn, DataSetType, LabelsType>>(h);
        break;
    }
    case ErrorFuncType::SSE:
   {
       ptr = std::make_shared<SSEFunction<HypothesisFn,
                                          DataSetType,
                                          LabelsType>>(h);
       break;
   }

#ifdef KERNEL_DEBUG
    default:
        assert(false && "Unrecognized Error Function Type");
#endif
    }

    return ptr;

}


template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
std::shared_ptr<FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>>
ErrFuncFactory::build(ErrorFuncType type, HypothesisFn& h, const RegularizerFn& r){

    auto ptr = std::shared_ptr<FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>>();
    switch(type){

     case ErrorFuncType::MSE:
    {
        ptr = std::make_shared<MSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>>>(h, r);
        break;
    }
    case ErrorFuncType::SSE:
   {
       ptr = std::make_shared<SSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>>>(h, r);
       break;
   }

#ifdef KERNEL_DEBUG
    default:
        assert(false && "Unrecognized Error Function Type");
#endif
    }

    return ptr;

}


}

#endif // ERROR_FUNCTION_BASE_H
