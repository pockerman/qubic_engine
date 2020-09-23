#ifndef SSE_FUNCTION_IMPL_H
#define SSE_FUNCTION_IMPL_H

namespace kernel
{

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
SSEFunction<HypothesisFn, DataSetType,
            LabelsType, RegularizerFn>::SSEFunction(const typename SSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::hypothesis_t& h)
    :
   FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>(),
   h_ptr_(&h),
   r_ptr_(nullptr)
{}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
SSEFunction<HypothesisFn, DataSetType,
            LabelsType, RegularizerFn>::SSEFunction(const typename SSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::hypothesis_t& h,
                                                    const typename SSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::regularizer_t& r)
    :
   FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>(),
   h_ptr_(&h),
   r_ptr_(&r)
{}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
typename SSEFunction<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::output_t
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

    return ResultHolder<real_t>(std::move(result), true);
}


template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
DynVec<real_t>
SSEFunction<HypothesisFn, DataSetType,
            LabelsType, RegularizerFn>::gradients(const DataSetType& dataset, const LabelsType& labels)const{

    DynVec<real_t> gradients(h_ptr_->n_coeffs(), 0.0);

    for(uint_t rowIdx=0; rowIdx<dataset.rows(); ++rowIdx){

        auto grad = gradient(get_row(dataset, rowIdx), labels[rowIdx]);
        gradients += grad;

        /*for(int coeff=0; coeff<h_ptr_->n_coeffs(); ++coeff){
            gradients[coeff] += grad[coeff];
        }*/
    }

    return gradients;
}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
template<typename RowTp, typename LabelTp>
DynVec<real_t>
SSEFunction<HypothesisFn, DataSetType,
            LabelsType, RegularizerFn>::gradient(const RowTp& row, const LabelTp& label)const{

    DynVec<real_t> grad(h_ptr_->n_coeffs(), 0.0);

    auto diff = (label - h_ptr_->value(row));
    auto hypothesis_grads = h_ptr_->coeff_grads(row);

    for(int coeff=0; coeff<h_ptr_->n_coeffs(); ++coeff){
            grad[coeff] += -2.0*diff*hypothesis_grads[coeff];
    }

    return grad;
}

///==============================================================================================

/// \brief base class for task description
namespace detail
{

template<typename HypothesisFn, typename DataSetType, typename LabelsType, typename ResultType>
struct task_description_base: public SimpleTaskBase<ResultType>
{

    typedef typename SimpleTaskBase<ResultType>::result_t result_t;

    /// \brief Constructor
    task_description_base(uint_t id, const PartitionedType<DataSetType>& data_set,
                          const PartitionedType<LabelsType>& labels, const HypothesisFn& h);

    /// \brief Reschedule the task. If called after this->operator()
    /// will override any TaskState has been set. By default it sets the
    /// task state to TaskState::PENDING
    virtual void reschedule()override final;

    /// \brief The pointer to the data set the task is working on
    const PartitionedType<DataSetType>* data_set_ptr_;

    /// \brief Pointer to the labels
    const PartitionedType<LabelsType>* labels_ptr_;

    /// \brief Pointer to the hypothesis function
    const HypothesisFn* h_ptr_;

};

template<typename HypothesisFn, typename DataSetType, typename LabelsType, typename ResultType>
task_description_base<HypothesisFn, DataSetType, LabelsType, ResultType>::task_description_base(uint_t id, const PartitionedType<DataSetType>& data_set,
                                                                                    const PartitionedType<LabelsType>& labels, const HypothesisFn& h)
    :
      SimpleTaskBase<ResultType>(id),
      data_set_ptr_(&data_set),
      labels_ptr_(&labels),
      h_ptr_(&h)
{}

template<typename HypothesisFn, typename DataSetType, typename LabelsType, typename ResultType>
void
task_description_base<HypothesisFn, DataSetType, LabelsType, ResultType>::reschedule(){

    this->result_.invalidate_result(true);
    this->set_state(TaskBase::TaskState::PENDING);
}

}

/// \brief Struct describing the task
/// of evaluating the MSE function
template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
struct SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::task_value_description: public detail::task_description_base<HypothesisFn, DataSetType, LabelsType, real_t>
{

public:

    /// \brief Constructor
    task_value_description(uint_t id, const PartitionedType<DataSetType>& data_set,
                         const PartitionedType<LabelsType>& labels, const HypothesisFn& h);

protected:

    /// \brief Execute the task
    virtual void run()override final;

};

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
SSEFunction<HypothesisFn,
            PartitionedType<DataSetType>,
            PartitionedType<LabelsType>, RegularizerFn>::task_value_description::task_value_description(uint_t id,
                                                                                                        const PartitionedType<DataSetType>& data_set,
                                                                                                        const PartitionedType<LabelsType>& labels,
                                                                                                        const HypothesisFn& h)
    :
      detail::task_description_base<HypothesisFn, DataSetType, LabelsType, real_t>(id, data_set,
                                                                           labels, h)
{}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
void
SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::task_value_description::run(){

    /// get the partitions associated with this task
    // get the rows partiton indeces corresponding to this task
    const auto parts = this->data_set_ptr_->get_partition(this->get_id());
    real_t result = 0.0;

    auto begin = parts.begin();
    auto end   = parts.end();

    for(uint_t r  = begin; r < end; ++r){

        auto row = get_row(*this->data_set_ptr_, r);
        auto diff = (*this->labels_ptr_)[r] - this->h_ptr_->value(row);
        diff *= diff;
        result += diff;
    }

    this->result_ += result;

    // this is a valid result
    this->result_.validate_result();
}



/// \brief Struct describing the task
/// of evaluating the MSE function gradients
template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
struct SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::task_gradient_value_description: public detail::task_description_base<HypothesisFn, DataSetType,
                                                                                                                                     LabelsType, DynVec<real_t>>
{

public:

    /// \brief Constructor
    task_gradient_value_description(uint_t id, const PartitionedType<DataSetType>& data_set,
                                    const PartitionedType<LabelsType>& labels, const HypothesisFn& h);

protected:

    /// \brief Execute the task
    virtual void run()override final;

};

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
SSEFunction<HypothesisFn,
            PartitionedType<DataSetType>,
            PartitionedType<LabelsType>, RegularizerFn>::task_gradient_value_description::task_gradient_value_description(uint_t id,
                                                                                                        const PartitionedType<DataSetType>& data_set,
                                                                                                        const PartitionedType<LabelsType>& labels,
                                                                                                        const HypothesisFn& h)
    :
   detail::task_description_base<HypothesisFn,
                                 DataSetType,
                                 LabelsType,
                                 DynVec<real_t>>(id, data_set, labels, h)
{}


template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
void
SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::task_gradient_value_description::run(){

    // get the rows partiton indeces corresponding to this task
    const auto parts = this->data_set_ptr_->get_partition(this->get_id());

    this->result_.get_resource() = DynVec<real_t>(this->h_ptr_->n_coeffs(), 0.0);

    // tmp result
    DynVec<real_t>& result = this->result_.get_resource();

    auto begin = parts.begin();
    auto end   = parts.end();

    for(uint_t r  = begin; r < end; ++r){

        auto row = get_row(*this->data_set_ptr_, r);
        auto diff = (*this->labels_ptr_)[r] - this->h_ptr_->value(row);
        DynVec<real_t> hypothesis_grads = this->h_ptr_->coeff_grads(row);

        for(uint_t c=0; c < this->h_ptr_->n_coeffs(); ++c){
            result[c] += -2.0*diff*hypothesis_grads[c];
        }
    }    
    // this is a valid result
    this->result_.validate_result();
}


template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::SSEFunction(const typename SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                                                                           PartitionedType<LabelsType>, RegularizerFn>::hypothesis_t& h)
    :
   FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>(),
   h_ptr_(&h),
   r_ptr_(nullptr),
   value_tasks_(),
   gradient_tasks_()
{}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
SSEFunction<HypothesisFn,
            PartitionedType<DataSetType>,
            PartitionedType<LabelsType>,
            RegularizerFn>::SSEFunction(const typename SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                                                                   PartitionedType<LabelsType>, RegularizerFn>::hypothesis_t& h,
                                        const typename SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                                                                   PartitionedType<LabelsType>, RegularizerFn>::regularizer_t& r)
    :
   FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>(),
   h_ptr_(&h),
   r_ptr_(&r),
   value_tasks_(),
   gradient_tasks_()
{}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
typename SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                     PartitionedType<LabelsType>, RegularizerFn>::output_t
SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::value(const DataSetType& dataset, const LabelsType& labels)const{

    if(r_ptr_){
       return SSEFunction<HypothesisFn, DataSetType, LabelsType,RegularizerFn>(*h_ptr_, *r_ptr_).value(dataset, labels);
    }

    return SSEFunction<HypothesisFn, DataSetType, LabelsType,RegularizerFn>(*h_ptr_).value(dataset, labels);
}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
DynVec<real_t>
SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::gradients(const DataSetType& dataset, const LabelsType& labels)const{

    if(r_ptr_){
       return SSEFunction<HypothesisFn, DataSetType,
                          LabelsType,RegularizerFn>(*h_ptr_, *r_ptr_).gradients(dataset, labels);
    }

    return SSEFunction<HypothesisFn, DataSetType,
                       LabelsType,RegularizerFn>(*h_ptr_).gradients(dataset, labels);
}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
template<typename Executor, typename Options>
typename SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
PartitionedType<LabelsType>, RegularizerFn>::output_t

SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
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
           break;
       }
       else{

           result += value_tasks_[t]->get_result();
       }
    }

    if(!result.is_result_valid()){
        return result;
    }

    if(r_ptr_){
        result += r_ptr_->value(executor, options, dataset, labels );
    }

    return result;

}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
template<typename Executor, typename Options>
ResultHolder<DynVec<real_t>>
SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
            PartitionedType<LabelsType>, RegularizerFn>::gradients(const PartitionedType<DataSetType>& dataset,
                                                                    const PartitionedType<LabelsType>& labels,
                                                                    Executor& executor, const Options& options){

    if(dataset.rows() != labels.size()){
       throw std::invalid_argument("Invalid number of data points and labels vector size");
    }

    if(gradient_tasks_.empty()){
        execute_gardient_value_tasks_(dataset, labels, executor, options);
    }
    else{

        for(uint_t t=0; t<gradient_tasks_.size(); ++t){
            gradient_tasks_[t]->reschedule();
        }

        /// execute the tasks
        executor.execute(gradient_tasks_, options);
    }

    // in any case now we have tasks that should have finished
    // by default we assume the result is valid
    ResultHolder<DynVec<real_t>> result(DynVec<real_t>(this->h_ptr_->n_coeffs(), 0.0), true);

    for(uint_t t=0; t < gradient_tasks_.size(); ++t){

        // if we reached here but for some reason the
        // task has not finished properly invalidate the result
       if(gradient_tasks_[t]->get_state() != kernel::TaskBase::TaskState::FINISHED){
           result.invalidate_result(false);
       }
       else{

           result += gradient_tasks_[t]->get_result();
       }
    }

    if(!result.is_result_valid()){
        return result;
    }

    return result;
}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
template<typename Executor, typename Options>
void
SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::execute_value_tasks_(const PartitionedType<DataSetType>& dataset,
                                                                                    const PartitionedType<LabelsType>& labels,
                                                                                    Executor& executor, const Options& options){

    // create the dot product tasks
    value_tasks_.reserve(executor.get_n_threads());

    typedef SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
            PartitionedType<LabelsType>, RegularizerFn>::task_value_type task_type;

    for(uint_t t=0; t<executor.get_n_threads(); ++t){
        value_tasks_.push_back(std::make_unique<task_type>(t, dataset, labels, *h_ptr_));
    }

    executor.execute(value_tasks_, options);
}

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
template<typename Executor, typename Options>
void
SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>::execute_gardient_value_tasks_(const PartitionedType<DataSetType>& dataset,
                                                                                             const PartitionedType<LabelsType>& labels,
                                                                                             Executor& executor, const Options& options){

    // create the dot product tasks
    gradient_tasks_.reserve(executor.get_n_threads());

    typedef SSEFunction<HypothesisFn, PartitionedType<DataSetType>,
            PartitionedType<LabelsType>, RegularizerFn>::task_gradient_type task_type;

    for(uint_t t=0; t<executor.get_n_threads(); ++t){
        gradient_tasks_.push_back(std::make_unique<task_type>(t, dataset, labels, *h_ptr_));
    }

    executor.execute(gradient_tasks_, options);
}


}

#endif // SSE_FUNCTION_IMPL_H
