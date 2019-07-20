#ifndef MATRIX_VECTOR_PRODUCT_H
#define MATRIX_VECTOR_PRODUCT_H

#include "parframe/base/types.h"
#include "parframe/executors/simple_task.h"
#include "parframe/executors/task_uitilities.h"
#include "parframe/utilities/result_holder.h"
#include "parframe/data_structs/range_1d.h"

#include <boost/noncopyable.hpp>

namespace kernel
{

/**
 * \brief Matrix-vector product
 */

template<typename MatTp, typename VecTp>
class MatVecProduct
{

public:

    typedef MatTp matrix_type;
    typedef VecTp vector_type;
    typedef ResultHolder<vector_type> result_type;

    /// \brief Constructor
    MatVecProduct(const matrix_type& mat, const vector_type& x);

    /// \brief Execute the matrix-vector product using the given executor.
    /// This function will delete any tasks that have been allocated and create new tasks every time is called.
    template<typename ExecutorTp>
    void execute(ExecutorTp& executor);

    /// \brief Execute the dot product of the given two vectors
    /// using the given executor. This function will not delete any tasks that have been
    /// allocated. Instead it will recompute these tasks.
    template<typename ExecutorTp>
    void reexecute(ExecutorTp& executor);

    /// \brief Returns true if the spawned tasks have finished
    bool tasks_finished()const{return taskutils::tasks_finished(tasks_);}

    /// \brief Returns the result
    const result_type& get()const{return result_;}

    /// \brief Attempt to get the result only if it is valid. It yields the calling thread
    /// as long as  the result is not valid
    const result_type& get_or_wait()const;

    /// \brief Attempt to get the result. If the result is not valid is waits for the
    /// specified time in milliseconds. It then returns the result regardless of its validity
    const result_type& get_or_wait_for(uint_t milliseconds)const;

private:

    struct mat_vec_product;

    /// \brief The spaned tasks
    std::vector<std::unique_ptr<mat_vec_product>> tasks_;

    /// \brief Pointers to vectors that product is to be computed
    const matrix_type* M_;
    const vector_type* x_;
    vector_type rslt_;

    /// \brief The result returned
    result_type result_;

    /// \brief Deallocate the memory for the tasks
    void clear_tasks_memory_();
};

template<typename MatTp, typename VecTp>
struct MatVecProduct<MatTp, VecTp>::mat_vec_product:  public parframe::SimpleTaskBase
{
   mat_vec_product(uint_t id, const MatTp& mat, const VecTp& x, VecTp& rslt)
       :
   parframe::SimpleTaskBase(id),
   mat_ptr(&mat),
   x_ptr(&x),
   rslt_ptr(&rslt)
   {}

   const MatTp* mat_ptr;
   const VecTp* x_ptr;
   VecTp* rslt_ptr;

   // execute the matrix-vector product
   virtual void run()override final;
};

template<typename MatTp, typename VecTp>
MatVecProduct<MatTp, VecTp>::MatVecProduct(const matrix_type& mat, const vector_type& x)
    :
   M_(&mat),
   x_(&x),
   rslt_(x.size(), 0.0),
   result_(rslt_)
{}


template<typename MatTp, typename VecTp>
template<typename ExecutorTp>
void
MatVecProduct<MatTp, VecTp>::execute(ExecutorTp& executor){


    if(!M_ || !x_ ){
        throw std::runtime_error("Null Matrix or vector pointers");
    }

    if(x_->size() != M_->columns()){
        throw std::runtime_error("Invalid Matrix-vector sizes");
    }

    // clear any memory it may have been allocated
    clear_tasks_memory_();

    // create the dot product tasks
    tasks_.reserve(executor.get_n_threads());

    typedef MatVecProduct<MatTp, VecTp>::mat_vec_product task_type;

    for(uint_t t=0; t<executor.get_n_threads(); ++t){

        tasks_.push_back(std::make_unique<task_type>(t, *M_, *x_, rslt_));
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

template<typename MatTp, typename VecTp>
template<typename ExecutorTp>
void
MatVecProduct<MatTp, VecTp>::reexecute(ExecutorTp& executor){

    if(tasks_.empty()){
        execute(executor);
    }
    else{

        typedef typename MatVecProduct<MatTp, VecTp>::mat_vec_product task_type;

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

template<typename MatTp, typename VecTp>
const typename MatVecProduct<MatTp, VecTp>::result_type&
MatVecProduct<MatTp, VecTp>::get_or_wait()const{
    result_.get_or_wait();
    return result_;
}

template<typename MatTp, typename VecTp>
const typename MatVecProduct<MatTp, VecTp>::result_type&
MatVecProduct<MatTp, VecTp>::get_or_wait_for(uint_t milliseconds)const{

    result_.get_or_wait_for(milliseconds);
    return result_;
}

template<typename MatTp, typename VecTp>
void
MatVecProduct<MatTp, VecTp>::clear_tasks_memory_(){

    if(tasks_.empty())
        return;

    using task_type = MatVecProduct<MatTp, VecTp>::mat_vec_product;

    for(auto& task: tasks_){
        task.reset(nullptr);
    }
}

template<typename MatTp, typename VecTp>
void
MatVecProduct<MatTp, VecTp>::mat_vec_product::run(){

    // get the rows partiton indeces corresponding to this task
    const auto parts = mat_ptr->get_partition(this->get_id());

    auto begin = parts.begin();
    auto end   = parts.end();

    for(uint_t r  = begin; r < end; ++r){
        auto& rslt = rslt_ptr->operator[](r);

        for(auto it = mat_ptr->begin(r); it != mat_ptr->end(r); ++it )  {
           rslt += *it*x_ptr->operator[](r);
        }
    }
}

}

#endif // MATRIX_VECTOR_PRODUCT_H
