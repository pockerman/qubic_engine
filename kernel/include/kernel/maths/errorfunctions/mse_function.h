#ifndef MSE_FUNCTION_H
#define MSE_FUNCTION_H

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

/**
 * @brief The MSEFunction class. Models the Mean Squred Error
 */
template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn=DummyFunction<real_t, DataSetType, LabelsType>>
class MSEFunction: public FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>
{
public:

    typedef typename FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>::output_t output_t;
    typedef HypothesisFn hypothesis_t;
    typedef RegularizerFn regularizer_t;

    /// \brief Constructor
    MSEFunction(const hypothesis_t& h);

    /// \brief Constructor
    MSEFunction(const hypothesis_t& h, const regularizer_t& r);

    /// \brief Returns the value of the function
    virtual output_t value(const DataSetType& dataset, const LabelsType& labels)const override final;

    /// \brief Returns the gradients of the function
    virtual DynVec<real_t> gradients(const DataSetType& dataset, const LabelsType& labels)const override final;

    /// \brief Returns the number of coefficients
    virtual uint_t n_coeffs()const override final{return 1;}

private:

    const hypothesis_t* h_ptr_;
    const regularizer_t* r_ptr_;

};

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
MSEFunction<HypothesisFn, DataSetType,
            LabelsType, RegularizerFn>::MSEFunction(const typename MSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::hypothesis_t& h)
    :
   FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>(),
   h_ptr_(&h),
   r_ptr_(nullptr)
{}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
MSEFunction<HypothesisFn, DataSetType,
            LabelsType, RegularizerFn>::MSEFunction(const typename MSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::hypothesis_t& h,
                                                    const typename MSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::regularizer_t& r)
    :
   FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>(),
   h_ptr_(&h),
   r_ptr_(&r)
{}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
typename MSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::output_t
MSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::value(const DataSetType& dataset, const LabelsType& labels)const{

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

    result /= dataset.rows();

    if(r_ptr_){
        result += r_ptr_->value(dataset, labels);
    }

    return ResultHolder<real_t>(std::move(result), true);
}


template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
DynVec<real_t>
MSEFunction<HypothesisFn, DataSetType,
            LabelsType, RegularizerFn>::gradients(const DataSetType& dataset, const LabelsType& labels)const{


}


/**
 * @brief The MSEFunction class. Models the Mean Squred Error
 */
template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
class MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>: public FunctionBase<ResultHolder<real_t>,
                                                                                   DataSetType,
                                                                                   LabelsType>
{
public:

    typedef typename FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>::output_t output_t;

    typedef HypothesisFn hypothesis_t;
    typedef RegularizerFn regularizer_t;

    /// \brief Constructor
    MSEFunction(const hypothesis_t& h);

    /// \brief Constructor
    MSEFunction(const hypothesis_t& h, const regularizer_t& r);

    /// \brief Returns the value of the function
    virtual output_t value(const DataSetType& dataset, const LabelsType& labels)const override final;

    /// \brief Returns the value of the function using the provided executor
    template<typename Executor, typename Options>
    output_t value(const PartitionedType<DataSetType>& dataset, const PartitionedType<LabelsType>& labels,
                   Executor& executor, const Options& options);

    /// \brief Returns the gradients of the function
    virtual DynVec<real_t> gradients(const DataSetType& dataset, const LabelsType& labels)const override final;

    /// \brief Returns the number of coefficients
    virtual uint_t n_coeffs()const override final{return 1;}

private:

    const hypothesis_t* h_ptr_;
    const regularizer_t* r_ptr_;


    template<typename Executor, typename Options>
    void execute_value_tasks_(const PartitionedType<DataSetType>& dataset, const PartitionedType<LabelsType>& labels,
                               Executor& executor, const Options& options);


    /// \brief Struct describing the task
    /// of evaluating the MSE function
    struct task_value_description: public SimpleTaskBase<real_t>
    {

    public:

        typedef SimpleTaskBase<real_t>::result_t result_t;

        /// \brief Constructor
        task_value_description(uint_t id, const PartitionedType<DataSetType>& data_set,
                             const PartitionedType<LabelsType>& labels, const HypothesisFn& h);

        /// \brief Reschedule the task. If called after this->operator()
        /// will override any TaskState has been set. By default it sets the
        /// task state to TaskState::PENDING
        virtual void reschedule()override final;

    protected:

        /// \brief Execute the task
        virtual void run()override final;

        /// \brief The pointer to the data set the task is working on
        const PartitionedType<DataSetType>* data_set_ptr_;

        /// \brief Pointer to the labels
        const PartitionedType<LabelsType>* labels_ptr_;

        /// \brief Pointer to the hypothesis function
        const HypothesisFn* h_ptr_;
    };

    typedef task_value_description task_value_type;

    /// \list of value tasks
    std::vector<std::unique_ptr<task_value_description>> value_tasks_;

};

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::MSEFunction(const typename MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                                                                           PartitionedType<LabelsType>, RegularizerFn>::hypothesis_t& h)
    :
   FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>(),
   h_ptr_(&h),
   r_ptr_(nullptr),
   value_tasks_()
{}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
MSEFunction<HypothesisFn,
            PartitionedType<DataSetType>,
            PartitionedType<LabelsType>,
            RegularizerFn>::MSEFunction(const typename MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                                                                   PartitionedType<LabelsType>, RegularizerFn>::hypothesis_t& h,
                                        const typename MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                                                                   PartitionedType<LabelsType>, RegularizerFn>::regularizer_t& r)
    :
   FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>(),
   h_ptr_(&h),
   r_ptr_(&r),
   value_tasks_()
{}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
typename MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                     PartitionedType<LabelsType>, RegularizerFn>::output_t

MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::value(const DataSetType& dataset, const LabelsType& labels)const{

    return MSEFunction<HypothesisFn, DataSetType, LabelsType,RegularizerFn>(*h_ptr_).value(dataset, labels);
}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>

template<typename Executor, typename Options>

typename MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
PartitionedType<LabelsType>, RegularizerFn>::output_t

MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::value(const PartitionedType<DataSetType>& dataset,
                                                                     const PartitionedType<LabelsType>& labels,
                                                                     Executor& executor, const Options& options){

    if(dataset.rows() != labels.size()){
       throw std::invalid_argument("Invalid number of data points and labels vector size");
    }

    if(value_tasks_.empty()){
        execute_value_tasks_(dataset, labels, executor, options);
    }
    else{

        for(uint_t t=0; t<executor.get_n_threads(); ++t){
            value_tasks_[t]->reschedule();
        }

        /// execute the tasks
        executor.execute(value_tasks_, options);
    }

    // in any case now we have tasks that should have finished
    // by default we assume the result is valid
    ResultHolder<real_t> result(0.0, true);

    for(uint_t t=0; t < value_tasks_.size(); ++t){

        // if we reached here but for some reason the
        // task has not finished properly invalidate the result
       if(value_tasks_[t]->get_state() != kernel::TaskBase::TaskState::FINISHED){
           result.invalidate_result(false);
       }
       else{

           result += value_tasks_[t]->get_result();
       }
    }

    if(!result.is_result_valid()){
        return result;
    }

    result /= dataset.rows();

    if(r_ptr_){
        result += r_ptr_->value(executor, options, dataset, labels );
    }

    return result;

}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
template<typename Executor, typename Options>
void
MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::execute_value_tasks_(const PartitionedType<DataSetType>& dataset,
                                                                                    const PartitionedType<LabelsType>& labels,
                                                                                    Executor& executor, const Options& options){

    // create the dot product tasks
    value_tasks_.reserve(executor.get_n_threads());

    typedef MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
            PartitionedType<LabelsType>, RegularizerFn>::task_value_type task_type;

    for(uint_t t=0; t<executor.get_n_threads(); ++t){
        value_tasks_.push_back(std::make_unique<task_type>(t, dataset, labels, *h_ptr_));
    }

    executor.execute(value_tasks_, options);

    //is the result valid
}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
DynVec<real_t>
MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::gradients(const DataSetType& dataset, const LabelsType& labels)const{


}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>

MSEFunction<HypothesisFn,
            PartitionedType<DataSetType>,
            PartitionedType<LabelsType>, RegularizerFn>::task_value_description::task_value_description(uint_t id,
                                                                                                        const PartitionedType<DataSetType>& data_set,
                                                                                                        const PartitionedType<LabelsType>& labels,
                                                                                                        const HypothesisFn& h)
    :
      SimpleTaskBase<real_t>(id),
      data_set_ptr_(&data_set),
      labels_ptr_(&labels),
      h_ptr_(&h)
{}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
void
MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::task_value_description::run(){

    /// get the partitions associated with this task
    // get the rows partiton indeces corresponding to this task
    const auto parts = data_set_ptr_->get_partition(this->get_id());
    real_t result = 0.0;

    auto begin = parts.begin();
    auto end   = parts.end();

    for(uint_t r  = begin; r < end; ++r){

        auto row = get_row(*data_set_ptr_, r);
        auto diff = (*labels_ptr_)[r] - h_ptr_->value(row);
        diff *= diff;
        result += diff;
    }

    this->result_ += result;

    // this is a valid result
    this->result_.validate_result();
}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
void
MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::task_value_description::reschedule(){

    this->result_.invalidate_result(true);
    set_state(TaskBase::TaskState::PENDING);
}

}

#endif // MSE_FUNCTION_H
