#ifndef DOT_PRODUCT_H
#define DOT_PRODUCT_H

/**
  *
  * Compute dot product between two vectors
  *
  *
  *
  */

#include "parframe/base/types.h"
#include "parframe/executors/simple_task.h"
#include "parframe/executors/task_uitilities.h"
#include "parframe/models/reduction_operations.h"

#include <boost/noncopyable.hpp>
#include <stdexcept>
#include <memory>

namespace parframe
{

template<typename VectorTp, typename ResultTp=typename VectorTp::value_type>
class DotProduct: private boost::noncopyable
{

public:

    typedef VectorTp vector_type;
    typedef Sum<ResultTp> result_type;

    /// \brief Constructor
    DotProduct();

    /// \brief Execute the reduction operation using the given partitions of the
    /// data and the given executor
    template<typename ExecutorTp>
    void execute(const VectorTp& v1, const VectorTp& v2, ExecutorTp& executor);

    /// \brief Returns true if the spawned tasks have finished
    bool tasks_finished()const{return taskutils::tasks_finished(tasks_);}

    /// \brief Returns the result
    const result_type get()const{return result_;}

    /// \brief Attempt to get the result only if it is valid. Perfroms busy
    /// wait is the result is not valid
    result_type get_or_wait()const;

private:

    /// \brief The actual dot product task
    struct DoDotProduct;

    /// \brief The spaned tasks
    std::vector<std::unique_ptr<DoDotProduct>> tasks_;

    /// \brief The result returned
    result_type result_;

    struct DoDotProduct: public SimpleTaskBase
    {

        /// \brief Constructor
        DoDotProduct(uint_t t, const VectorTp& v1, const VectorTp& v2);

        /// \brief Returns the local task result
        const result_type get_result()const{return result;}

    protected:

        /// \brief Override base class run method
        virtual void run()override final;

        const VectorTp* v1_ptr;
        const VectorTp* v2_ptr;
        result_type result;
    };
};

template<typename VectorTp, typename ResultTp>
DotProduct<VectorTp,ResultTp>::DoDotProduct::DoDotProduct(uint_t t, const VectorTp& v1, const VectorTp& v2)
    :
      SimpleTaskBase(t),
      v1_ptr(&v1),
      v2_ptr(&v2),
      result()
{}


template<typename VectorTp, typename ResultTp>
void
DotProduct<VectorTp, ResultTp>::DoDotProduct::run(){

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

template<typename VectorTp, typename ResultTp>
DotProduct<VectorTp, ResultTp>::DotProduct()
    :
 tasks_(),
  result_()
{}

template<typename VectorTp, typename ResultTp>
template<typename ExecutorTp>
void
DotProduct<VectorTp, ResultTp>::execute(const VectorTp& v1, const VectorTp& v2, ExecutorTp& executor){

    if(v1.size() != v2.size()){
        throw std::runtime_error("Invalid vector sizes");
    }

    // create the dot product tasks
    tasks_.reserve(executor.get_n_threads());

    typedef DotProduct<VectorTp, ResultTp>::DoDotProduct task_type;

    for(uint_t t=0; t<executor.get_n_threads(); ++t){

        tasks_.push_back(std::make_unique<task_type>(t, v1, v2));
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
       if(tasks_[t]->get_state() != TaskBase::TaskState::FINISHED){
           result_.invalidate_result();
       }
       else{
           result_.join(static_cast<task_type*>(tasks_[t].get())->get_result());
       }
    }
}

template<typename VectorTp, typename ResultTp>
typename DotProduct<VectorTp, ResultTp>::result_type
DotProduct<VectorTp, ResultTp>::get_or_wait()const{

    while(!result_){
        std::this_thread::yield();
    }

    return result_;
}


}


#endif // DOT_PRODUCT_H
