#ifndef VECTOR_UPDATER_H
#define VECTOR_UPDATER_H

/**
  * Implements axpy operation on the given vector.
  * Thus, it implements the following update
  * x = a*y op b*z
  *
  * The vectors x, y, z must all have the same length
  */
#include "parframe/base/types.h"
#include "parframe/executors/simple_task.h"
#include "parframe/executors/task_uitilities.h"
#include "parframe/utilities/result_holder.h"

#include <boost/noncopyable.hpp>
#include <stdexcept>
#include <memory>

namespace kernel
{

template<typename VectorTp, typename OpTp, typename FactorTp=typename VectorTp::value_type>
class VectorUpdater: private boost::noncopyable
{

public:

    typedef VectorTp vector_type;
    typedef ResultHolder<vector_type> result_type;
    typedef FactorTp factor_type;
    typedef OpTp operator_type;

    /// \brief Default Constructor
    VectorUpdater()=delete;

    /// \brief Constructor
    VectorUpdater(VectorTp& x, const VectorTp& y, const VectorTp& z,
                  const FactorTp& a, const FactorTp& b);

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
    bool tasks_finished()const{return parframe::taskutils::tasks_finished(tasks_);}

    /// \brief Returns the result
    const result_type& get()const{return result_;}

    /// \brief Attempt to get the result only if it is valid. It yields the calling thread
    /// as long as  the result is not valid
    const result_type& get_or_wait()const;

    /// \brief Attempt to get the result. If the result is not valid is waits for the
    /// specified time in milliseconds. It then returns the result regardless of its validity
    const result_type& get_or_wait_for(uint_t milliseconds)const;

private:

    /// \brief The actual dot product task
    struct UpdateVector;

    /// \brief The spaned tasks
    std::vector<std::unique_ptr<UpdateVector>> tasks_;

    /// \brief The result returned
    result_type result_;

    /// \brief Pointers to vectors that dot product is to be computed
    VectorTp* x_;
    const VectorTp* y_;
    const VectorTp* z_;
    const FactorTp* a_;
    const FactorTp* b_;

    /// \brief Deallocate the memory for the tasks
    void clear_tasks_memory_();

    struct UpdateVector: public parframe::SimpleTaskBase
    {

        /// \brief Constructor
        UpdateVector(uint_t t, VectorTp& x, const VectorTp& y, const VectorTp& z,
                     const FactorTp& a, const FactorTp& b);

    protected:

        /// \brief Override base class run method
        virtual void run()override final;

        VectorTp* x_;
        const VectorTp* y_;
        const VectorTp* z_;
        const FactorTp* a_;
        const FactorTp* b_;
    };
};

template<typename VectorTp, typename OpTp, typename FactorTp>
VectorUpdater<VectorTp, OpTp, FactorTp>::VectorUpdater(VectorTp& x, const VectorTp& y,
                                                       const VectorTp& z, const FactorTp& a, const FactorTp& b)
    :
tasks_(),
result_(x),
x_(&x),
y_(&y),
z_(&z),
a_(&a),
b_(&b)
{}


template<typename VectorTp, typename OpTp, typename FactorTp>
template<typename ExecutorTp>
void
VectorUpdater<VectorTp, OpTp, FactorTp>::execute(ExecutorTp& executor){


    if(!x_ || !y_ || !z_){
        throw std::runtime_error("Null vector pointers");
    }

    if(!a_ || !b_ ){
        throw std::runtime_error("Null factor pointers");
    }

    if(x_->size() != y_->size() && x_->size() != z_->size()){
        throw std::runtime_error("Invalid vector sizes");
    }

    // clear any memory it may have been allocated
    clear_tasks_memory_();

    // create the dot product tasks
    tasks_.reserve(executor.get_n_threads());

    typedef VectorUpdater<VectorTp, OpTp, FactorTp>::UpdateVector task_type;

    for(uint_t t=0; t<executor.get_n_threads(); ++t){

        tasks_.push_back(std::make_unique<task_type>(t, *x_, *y_, *z_, *a_, *b_));
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
       if(tasks_[t]->get_state() != parframe::TaskBase::TaskState::FINISHED){
           result_.invalidate_result();
       }
    }
}

template<typename VectorTp, typename OpTp, typename FactorTp>
template<typename ExecutorTp>
void
VectorUpdater<VectorTp, OpTp, FactorTp>::reexecute(ExecutorTp& executor){

    if(tasks_.empty()){

        execute(executor);
    }
    else{

        typedef VectorUpdater<VectorTp, OpTp, FactorTp>::UpdateVector task_type;

        for(uint_t t=0; t<executor.get_n_threads(); ++t){

            tasks_[t]->set_state(parframe::TaskBase::TaskState::PENDING);
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
           if(tasks_[t]->get_state() != parframe::TaskBase::TaskState::FINISHED){
               result_.invalidate_result();
           }
        }
    }
}

template<typename VectorTp, typename OpTp, typename FactorTp>
const typename VectorUpdater<VectorTp, OpTp, FactorTp>::result_type&
VectorUpdater<VectorTp, OpTp, FactorTp>::get_or_wait()const{
    result_.get_or_wait();
    return result_;
}

template<typename VectorTp, typename OpTp, typename FactorTp>
const typename VectorUpdater<VectorTp, OpTp, FactorTp>::result_type&
VectorUpdater<VectorTp, OpTp, FactorTp>::get_or_wait_for(uint_t milliseconds)const{

    result_.get_or_wait_for(milliseconds);
    return result_;
}

template<typename VectorTp, typename OpTp, typename FactorTp>
void
VectorUpdater<VectorTp, OpTp, FactorTp>::clear_tasks_memory_(){

    if(tasks_.empty())
        return;

    using task_type = VectorUpdater<VectorTp, OpTp, FactorTp>::UpdateVector;

    for(auto& task: tasks_){
        task.reset(nullptr);
    }
}


template<typename VectorTp, typename OpTp, typename FactorTp>
VectorUpdater<VectorTp, OpTp, FactorTp>::UpdateVector::UpdateVector(uint_t t, VectorTp& x, const VectorTp& y, const VectorTp& z,
                                                                    const FactorTp& a, const FactorTp& b)
    :
SimpleTaskBase(t),
x_(&x),
y_(&y),
z_(&z),
a_(&a),
b_(&b)
{}


template<typename VectorTp, typename OpTp, typename FactorTp>
void
VectorUpdater<VectorTp, OpTp, FactorTp>::UpdateVector::run(){

    // get the rows partiton indeces corresponding to this task
    const auto parts = x_->get_partition(this->get_id());

    auto begin = parts.begin();
    auto end   = parts.end();

    for(uint_t r  = begin; r < end; ++r){
        (*x_)[r] = OpTp::get((*y_)[r], *a_, (*z_)[r], *b_ );
    }
}


}

#endif // VECTOR_UPDATER_H
