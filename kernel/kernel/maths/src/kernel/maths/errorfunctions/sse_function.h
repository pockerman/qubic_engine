#ifndef SSE_FUNCTION_H
#define SSE_FUNCTION_H

#include "kernel/base/types.h"
#include "kernel/maths/functions/function_base.h"
#include "kernel/maths/functions/dummy_function.h"
#include "kernel/maths/matrix_utilities.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/parallel/utilities/result_holder.h"
#include "kernel/parallel/utilities/partitioned_type.h"

#include <iostream>
#include <exception>
#include <vector>

namespace kernel
{

///
/// \brief The SSEFunction class. Models the Sum Squred Error
///
template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn=DummyFunction<real_t, DataSetType, LabelsType>>
class SSEFunction: public FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>
{
public:

    typedef typename FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>::output_t output_t;
    typedef HypothesisFn hypothesis_t;
    typedef RegularizerFn regularizer_t;

    ///
    /// \brief Constructor
    ///
    SSEFunction(hypothesis_t& h);

    ///
    /// \brief Constructor
    ///
    SSEFunction(hypothesis_t& h, const regularizer_t& r);

    ///
    /// \brief Returns the value of the function
    ///
    virtual output_t value(const DataSetType& dataset,
                           const LabelsType& labels)const override final;

    ///
    /// \brief Returns the gradients of the function
    ///
    virtual DynVec<real_t> gradients(const DataSetType& dataset, const LabelsType& labels)const override final;

    ///
    /// \brief Returns the gradient of the function
    /// on the given point
    ///
    template<typename RowTp, typename LabelTp>
    DynVec<real_t> gradient(const RowTp& row, const LabelTp& label)const;

    ///
    /// \brief Returns the number of coefficients
    ///
    virtual uint_t n_coeffs()const override final{return h_ptr_->n_coeffs();}

    ///
    /// \brief Update the underlying model
    ///
    template<typename VectorContainerTp>
    void update_model(const VectorContainerTp& coeffs);

    ///
    /// \brief coeffs. Returns the coefficients of the underlying model
    ///
    DynVec<real_t> coeffs()const override final{return h_ptr_->coeffs();}

    ///
    /// \brief update_coeffs
    /// \param params
    ///
    virtual void update_coeffs(const DynVec<real_t>& params) override final{update_model(params);};

private:

    hypothesis_t* h_ptr_;
    const regularizer_t* r_ptr_;

};

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
class SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>: public FunctionBase<ResultHolder<real_t>,
                                                                                   DataSetType,
                                                                                   LabelsType>
{
public:

    typedef typename FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>::output_t output_t;

    typedef HypothesisFn hypothesis_t;
    typedef RegularizerFn regularizer_t;

    /// \brief Constructor
    SSEFunction(hypothesis_t& h);

    /// \brief Constructor
    SSEFunction(hypothesis_t& h, const regularizer_t& r);

    /// \brief Returns the value of the function
    virtual output_t value(const DataSetType& dataset, const LabelsType& labels)const override final;

    /// \brief Returns the value of the function using the provided executor
    template<typename Executor, typename Options>
    output_t value(const PartitionedType<DataSetType>& dataset, const PartitionedType<LabelsType>& labels,
                   Executor& executor, const Options& options);

    /// \brief Returns the gradients of the function
    virtual DynVec<real_t> gradients(const DataSetType& dataset, const LabelsType& labels)const override final;

    /// \brief Returns the gradients of the function with respect to the
    /// coefficients of the hypothesis function
    template<typename Executor, typename Options>
    ResultHolder<DynVec<real_t>> gradients(const PartitionedType<DataSetType>& dataset,
                                           const PartitionedType<LabelsType>& labels,
                                            Executor& executor, const Options& options);

    /// \brief Returns the number of coefficients
    virtual uint_t n_coeffs()const override final{return 1;}

private:

    hypothesis_t* h_ptr_;
    const regularizer_t* r_ptr_;

    template<typename Executor, typename Options>
    void execute_value_tasks_(const PartitionedType<DataSetType>& dataset,
                              const PartitionedType<LabelsType>& labels,
                               Executor& executor, const Options& options);

    template<typename Executor, typename Options>
    void execute_gardient_value_tasks_(const PartitionedType<DataSetType>& dataset,
                                       const PartitionedType<LabelsType>& labels,
                                       Executor& executor, const Options& options);

    /// \brief Struct describing the task
    /// of evaluating the MSE function
    struct task_value_description;
    typedef task_value_description task_value_type;

    struct task_gradient_value_description;
    typedef task_gradient_value_description task_gradient_type;

    /// \list of value tasks
    std::vector<std::unique_ptr<task_value_type>> value_tasks_;

    /// \list of gradient tasks
    std::vector<std::unique_ptr<task_gradient_type>> gradient_tasks_;

};

}

#include "kernel/maths/errorfunctions/sse_function_impl.h"

#endif // SSE_FUNCTION_H
