#ifndef MSE_ERROR_FUNCTION_H
#define MSE_ERROR_FUNCTION_H

#include "kernel/maths/functions/function_base.h"
#include "kernel/maths/matrix_utilities.h"
#include <exception>

namespace kernel {

template<typename HypothesisTp, typename DataSetTp, typename LabelsTp, typename RegularizerTp>
class MSEErrorFuncction: public FunctionBase<real_t, DataSetTp, LabelsTp>
{

public:

    typedef HypothesisTp hypothesis_t;
    typedef DataSetTp dataset_t;
    typedef LabelsTp labels_t;
    typedef RegularizerTp regularizer_t;
    typedef typename FunctionBase<real_t, DataSetTp, LabelsTp>::output_t output_t;

    /// \brief Constructor
    MSEErrorFuncction(const hypothesis_t& hypothesis );

    /// \brief Constructor. Create using both a hypothesis function
    /// and a regularizer
    MSEErrorFuncction(const hypothesis_t& hypothesis, const regularizer_t& regularizer );

    /**
     * Returns the value of the function
     */
    virtual output_t value(const dataset_t& dataset, const labels_t& labels)const override final;

    /**
      * Returns the gradients of the function
      */
    virtual DynVec<real_t> gradients(const dataset_t& dataset, const labels_t& labels)const override final;

    /**
      * Returns the number of coefficients
      */
    virtual uint_t n_coeffs()const override final{return h_ptr_->n_coeffs();};
    
    
private:

    /// \brief Pointer to the hypothesis function
    const hypothesis_t* h_ptr_;

    /// \brief Pointer to the regularizer used
    const regularizer_t* regularizer_ptr_;
};

template<typename HypothesisTp, typename DataSetTp, typename LabelsTp, typename RegularizerTp>
MSEErrorFuncction<HypothesisTp, DataSetTp, LabelsTp, RegularizerTp>::MSEErrorFuncction(const HypothesisTp& hypothesis)
    :
    FunctionBase<real_t, DataSetTp, LabelsTp>(),
    h_ptr_(&hypothesis),
    regularizer_ptr_(nullptr)
{}

template<typename HypothesisTp, typename DataSetTp, typename LabelsTp, typename RegularizerTp>
MSEErrorFuncction<HypothesisTp, DataSetTp,
                  LabelsTp, RegularizerTp>::MSEErrorFuncction(const hypothesis_t& hypothesis, const regularizer_t& regularizer )
    :
      FunctionBase<real_t, DataSetTp, LabelsTp>(),
      h_ptr_(&hypothesis),
      regularizer_ptr_(&regularizer)
{}

template<typename HypothesisTp, typename DataSetTp, typename LabelsTp, typename RegularizerTp>
typename MSEErrorFuncction<HypothesisTp, DataSetTp, LabelsTp, RegularizerTp>::output_t
MSEErrorFuncction<HypothesisTp, DataSetTp,
                  LabelsTp, RegularizerTp>::value(const dataset_t& dataset, const labels_t& labels)const{

    typedef typename  MSEErrorFuncction<HypothesisTp, DataSetTp, LabelsTp, RegularizerTp>::output_t output_t;

    if(dataset.m() != labels.size()){
        throw std::logic_error("Invalid number of data points and labels vector size");
    }

    output_t result = output_t();

    for(uint_t rowIdx=0; rowIdx<dataset.m(); ++rowIdx){

       auto row = get_row(dataset, rowIdx);
       real_t diff = labels.get(rowIdx) - h_ptr_->value(row);
       diff *= diff;
       result += diff;
    }

    result /= dataset.m();

    if(regularizer_ptr_){
       result += regularizer_ptr_->value(dataset, labels);
    }

    return result;
}

template<typename HypothesisTp, typename DataSetTp, typename LabelsTp, typename RegularizerTp>
DynVec<real_t>
MSEErrorFuncction<HypothesisTp, DataSetTp,
LabelsTp, RegularizerTp>::gradients(const dataset_t& dataset, const labels_t& labels)const{

    DynVec<real_t> gradients(h_ptr_->n_coeffs(), 0.0);

    for(uint_t rowIdx=0; rowIdx<dataset.m(); ++rowIdx){

        auto row = get_row(dataset, rowIdx);
        auto diff = (labels[rowIdx] - h_ptr_->value(row));
        auto hypothesis_grads = h_ptr_->coeff_gradients(row);

        for(int coeff=0; coeff<h_ptr_->n_coeffs(); ++coeff){
            gradients[coeff] += (-2.0/dataset.m())*diff*hypothesis_grads[coeff];
        }
    }

    return gradients;
}

}

#endif // MSE_ERROR_FUNCTION_H
