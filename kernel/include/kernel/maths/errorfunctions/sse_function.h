#ifndef SSE_FUNCTION_H
#define SSE_FUNCTION_H

#include "kernel/base/types.h"
#include "kernel/maths/functions/function_base.h"
#include "kernel/maths/functions/dummy_function.h"
#include "kernel/maths/matrix_utilities.h"

#include <exception>

namespace kernel
{

/**
 * @brief The SSEFunction class. Models the Sum Squred Error
 */
template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn=DummyFunction<real_t, DataSetType, LabelsType>>
class SSEFunction: public FunctionBase<real_t, DataSetType, LabelsType>
{
public:

    typedef typename FunctionBase<real_t, DataSetType, LabelsType>::output_t output_t;
    typedef HypothesisFn hypothesis_t;
    typedef RegularizerFn regularizer_t;

    /// \brief Constructor
    SSEFunction(const hypothesis_t& h);

    /// \brief Constructor
    SSEFunction(const hypothesis_t& h, const regularizer_t& r);


    /**
     * Returns the value of the function
     */
    virtual output_t value(const DataSetType& dataset, const LabelsType& labels)const override final;

    /**
      * Returns the gradients of the function
      */
    virtual DynVec<real_t> gradients(const DataSetType& dataset, const LabelsType& labels)const override final;

    /**
      * Returns the number of coefficients
      */
    virtual uint_t n_coeffs()const override final{return 1;}

private:

    const hypothesis_t* h_ptr_;
    const regularizer_t* r_ptr_;
};

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
SSEFunction<HypothesisFn, DataSetType,
            LabelsType, RegularizerFn>::SSEFunction(const typename SSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::hypothesis_t& h)
    :
   FunctionBase<real_t, DataSetType, LabelsType>(),
   h_ptr_(&h),
   r_ptr_(nullptr)
{}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
SSEFunction<HypothesisFn, DataSetType,
            LabelsType, RegularizerFn>::SSEFunction(const typename SSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::hypothesis_t& h,
                                                    const typename SSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::regularizer_t& r)
    :
   FunctionBase<real_t, DataSetType, LabelsType>(),
   h_ptr_(&h),
   r_ptr_(&r)
{}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
typename SSEFunction<HypothesisFn, DataSetType,
                     LabelsType, RegularizerFn>::output_t
SSEFunction<HypothesisFn, DataSetType,
            LabelsType, RegularizerFn>::value(const DataSetType& dataset, const LabelsType& labels)const{

    if(dataset.rows() != labels.size()){
       throw std::invalid_argument("Invalid number of data points and labels vector size");
    }

    real_t result = 0.0;

    for(uint_t r_idx=0; r_idx<dataset.rows(); ++r_idx){

        auto row = get_row(dataset, r_idx);
        auto diff = labels[r_idx] - h_ptr_->value(row);
        diff *= diff;
        result += diff;
    }

    if(r_ptr_){
        result += r_ptr_->value(dataset, labels);
    }

    return result;
}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
DynVec<real_t>
SSEFunction<HypothesisFn, DataSetType,
            LabelsType, RegularizerFn>::gradients(const DataSetType& dataset, const LabelsType& labels)const{


}


}

#endif // SSE_FUNCTION_H
