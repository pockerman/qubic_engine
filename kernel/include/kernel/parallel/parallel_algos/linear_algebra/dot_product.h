#ifndef DOT_PRODUCT_H
#define DOT_PRODUCT_H

/**
  *
  * Compute dot product between two vectors
  */

#include "kernel/base/types.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/parallel/threading/task_uitilities.h"
#include "kernel/parallel/utilities/reduction_operations.h"
#include "kernel/parallel/utilities/result_holder.h"

#include <boost/noncopyable.hpp>
#include <stdexcept>
#include <memory>

namespace kernel
{

template<typename VectorTp, typename ResultTp=typename VectorTp::value_type>
class DotProduct: private boost::noncopyable
{

public:
    typedef VectorTp vector_type;
    typedef Sum<ResultTp> result_type;

    /// \brief Default Constructor
    DotProduct();

    /// \brief Constructor
    DotProduct(const VectorTp& v1, const VectorTp& v2);

    /// \brief Constructor
    DotProduct(const VectorTp& v1);

    /// \brief Constructor
    DotProduct(const ResultHolder<VectorTp>& v1, const ResultHolder<VectorTp>& v2);

    /// \brief Constructor
    DotProduct(const ResultHolder<VectorTp>& v1);

    /// \brief Execute the dot product of the given two vectors
    /// using the given executor. This function will delete any tasks that have been
    /// allocated and create new tasks every time is called.
    template<typename ExecutorTp>
    void execute(ExecutorTp& executor);

    /// \brief Execute the dot product of the given two vectors
    /// using the given executor. This function will not delete any tasks that have been
    /// allocated. Instead it will recompute these tasks.
    template<typename ExecutorTp>
    void reexecute(ExecutorTp& executor);

    /// \brief Returns true if the spawned tasks have finished
    bool tasks_finished()const{return taskutils::tasks_finished(tasks_);}

    /// \brief Set the vectors over which to compute the dot product
    void set_vectors(const VectorTp& v1, const VectorTp& v2);

    /// \brief Returns the result
    const result_type& get()const{return result_;}

    /// \brief Attempt to get the result only if it is valid. Perfroms busy
    /// wait is the result is not valid
    const result_type& get_or_wait()const;

private:

    /// \brief The actual dot product task
    struct dot_product;

    /// \brief The spaned tasks
    std::vector<std::unique_ptr<dot_product>> tasks_;

    /// \brief The result returned
    result_type result_;

    /// \brief Pointers to vectors that dot product is to be computed
    const VectorTp* v1_ptr_;
    const VectorTp* v2_ptr_;

    /// \brief Deallocate the memory for the tasks
    void clear_tasks_memory_();

    struct dot_product: public kernel::SimpleTaskBase
    {

        /// \brief Constructor
        dot_product(uint_t t, const VectorTp& v1, const VectorTp& v2);

        /// \brief Returns the local task result
        const result_type& get_result()const{return result;}

        /// \brief Invalidate the result
        void invalidate_result(){result.invalidate_result(true);}

    protected:

        /// \brief Override base class run method
        virtual void run()override final;

        const VectorTp* v1_ptr;
        const VectorTp* v2_ptr;
        result_type result;
    };
};

template<typename VectorTp, typename ResultTp>
DotProduct<VectorTp, ResultTp>::DotProduct()
    :
tasks_(),
result_(),
v1_ptr_(nullptr),
v2_ptr_(nullptr)
{}

template<typename VectorTp, typename ResultTp>
DotProduct<VectorTp, ResultTp>::DotProduct(const VectorTp& v1, const VectorTp& v2)
    :
tasks_(),
result_(),
v1_ptr_(&v1),
v2_ptr_(&v2)
{}

template<typename VectorTp, typename ResultTp>
DotProduct<VectorTp, ResultTp>::DotProduct(const VectorTp& v1)
    :
tasks_(),
result_(),
v1_ptr_(&v1),
v2_ptr_(&v1)
{}

template<typename VectorTp, typename ResultTp>
DotProduct<VectorTp, ResultTp>::DotProduct(const ResultHolder<VectorTp>& v1)
    :
tasks_(),
result_(),
v1_ptr_(&v1.get_resource()),
v2_ptr_(&v1.get_resource())
{}

template<typename VectorTp, typename ResultTp>
DotProduct<VectorTp, ResultTp>::DotProduct(const ResultHolder<VectorTp>& v1, const ResultHolder<VectorTp>& v2)
    :
tasks_(),
result_(),
v1_ptr_(&v1.get_resource()),
v2_ptr_(&v2.get_resource())
{}


template<typename VectorTp, typename ResultTp>
void
DotProduct<VectorTp, ResultTp>::set_vectors(const VectorTp& v1, const VectorTp& v2){

    v1_ptr_ = &v1;
    v2_ptr_ = &v2;
}

template<typename VectorTp, typename ResultTp>
template<typename ExecutorTp>
void
DotProduct<VectorTp, ResultTp>::execute(ExecutorTp& executor){


    if(!v1_ptr_ || !v2_ptr_){
        throw std::runtime_error("Null vector pointers");
    }

    if(v1_ptr_->size() != v2_ptr_->size()){
        throw std::runtime_error("Invalid vector sizes");
    }

    //invalidate the result
    // TODO result should be invalidated only if requested
    // by the application
    result_.invalidate_result(true);

    // clear any memory it may have been allocated
    clear_tasks_memory_();

    // create the dot product tasks
    tasks_.reserve(executor.get_n_threads());

    typedef DotProduct<VectorTp, ResultTp>::dot_product task_type;

    for(uint_t t=0; t<executor.get_n_threads(); ++t){
        tasks_.push_back(std::make_unique<task_type>(t, *v1_ptr_, *v2_ptr_));
        executor.add_task(*(tasks_[t].get()));
    }

    // if the tasks have not finished yet
    // then the calling thread waits here
    while(!tasks_finished()){
       std::this_thread::yield();
    }

    //validate the result
    result_.validate_result();

    for(uint_t t=0; t < tasks_.size(); ++t){

        // if we reached here but for some reason the
        // task has not finished properly invalidate the result
       if(tasks_[t]->get_state() != kernel::TaskBase::TaskState::FINISHED){
           result_.invalidate_result(false);
       }
       else{
           result_.join(static_cast<task_type*>(tasks_[t].get())->get_result());
       }
    }
}

template<typename VectorTp, typename ResultTp>
template<typename ExecutorTp>
void
DotProduct<VectorTp, ResultTp>::reexecute(ExecutorTp& executor){

    if(tasks_.empty()){

        execute(executor);
    }
    else{

        //invalidate the result
        result_.invalidate_result(true);

        typedef DotProduct<VectorTp, ResultTp>::dot_product task_type;

        for(uint_t t=0; t<executor.get_n_threads(); ++t){

            tasks_[t]->invalidate_result();
            tasks_[t]->set_state(kernel::TaskBase::TaskState::PENDING);
            executor.add_task(*(tasks_[t].get()));
        }

        // if the tasks have not finished yet
        // then the calling thread waits here
        while(!tasks_finished()){
           std::this_thread::yield();
        }

        //validate the result
        result_.validate_result();

        for(uint_t t=0; t < tasks_.size(); ++t){

            // if we reached here but for some reason the
            // task has not finished properly invalidate the result
           if(tasks_[t]->get_state() != kernel::TaskBase::TaskState::FINISHED){
               result_.invalidate_result(false);
           }
           else{
               result_.join(static_cast<task_type*>(tasks_[t].get())->get_result());
           }
        }
    }
}

template<typename VectorTp, typename ResultTp>
const typename DotProduct<VectorTp, ResultTp>::result_type&
DotProduct<VectorTp, ResultTp>::get_or_wait()const{

    while(!result_.is_result_valid()){
        std::this_thread::yield();
    }

    return result_;
}

template<typename VectorTp, typename ResultTp>
void
DotProduct<VectorTp, ResultTp>::clear_tasks_memory_(){

    if(tasks_.empty())
        return;

    for(auto& task: tasks_){
        task.reset(nullptr);
    }
}

template<typename VectorTp, typename ResultTp>
DotProduct<VectorTp,ResultTp>::dot_product::dot_product(uint_t t, const VectorTp& v1, const VectorTp& v2)
    :
      SimpleTaskBase(t),
      v1_ptr(&v1),
      v2_ptr(&v2),
      result()
{}


template<typename VectorTp, typename ResultTp>
void
DotProduct<VectorTp, ResultTp>::dot_product::run(){

    // get the rows partiton indeces corresponding to this task
    const auto parts = v1_ptr->get_partition(this->get_id());

    auto begin = parts.begin();
    auto end   = parts.end();

    for(uint_t r  = begin; r < end; ++r){
        result.join((*v1_ptr)[r]*(*v2_ptr)[r]);
    }

    // this is a valid result
    result.validate_result();
}

}


#endif // DOT_PRODUCT_H
