#ifndef PARALLEL_FOR_H
#define PARALLEL_FOR_H

#include "kernel/base/config.h"
#include "kernel/base/exceptions.h"
#include "kernel/parallel/threading/iterate_task.h"
#include "kernel/parallel/threading/task_uitilities.h"
#include "kernel/parallel/utilities/result_holder.h"

#include "boost/noncopyable.hpp"

namespace kernel
{

namespace detail
{

template<typename RangeTp, typename BodyTp>
class parallel_for: private boost::noncopyable
{
public:

    typedef RangeTp range_type;
    typedef BodyTp  body_type;
    typedef IterateTask<typename range_type::partition_type, body_type, range_type> task_type;

    /// \brief Constructor
    parallel_for(range_type& range, const BodyTp& body);


    /// \brief Execute the parallel_for algorithm using the given executor
    template<typename ExecutorTp>
    ResultHolder<void> execute(ExecutorTp& executor);


    /// \brief Returns true if the spawned tasks have finished
    bool tasks_finished()const{return kernel::taskutils::tasks_finished(tasks_);}

private:

    /// \brief The range over which the algorithm works
    range_type& range_;


    /// \brief The operation applied on the range elements
    const body_type& body_;


    /// \brief The tasks to be submitted to the executor
    std::vector<std::unique_ptr<task_type>> tasks_;


    /// \brief The result of the computation
    ResultHolder<void> result_;

};


template<typename RangeTp, typename BodyTp>
parallel_for<RangeTp, BodyTp>::parallel_for(RangeTp& range, const BodyTp& body)
    :
range_(range),
body_(body)
{}


/// \brief Execute the parallel_for algorithm using the given executor

template<typename RangeTp, typename BodyTp>
template<typename ExecutorTp>
ResultHolder<void>
parallel_for<RangeTp, BodyTp>::execute(ExecutorTp& executor){


    typedef typename  parallel_for<RangeTp, BodyTp>::task_type task_type;
    tasks_.reserve(executor.get_n_threads());

    //spawn the tasks
    for(uint_t t = 0; t < executor.get_n_threads(); ++t){
        tasks_.push_back(std::make_unique<task_type>(t, range_.get_partition(t), body_, range_));
        executor.add_task(*(tasks_[t].get()));
    }

    // if the tasks have not finished yet
    // then the calling thread waits here
    while(!tasks_finished()){
       std::this_thread::yield();
    }


    result_.validate_result();
    for(uint_t t=0; t < tasks_.size(); ++t){

        // if we reached here but for some reason the
        // task has not finished properly invalidate the result
       if(tasks_[t]->get_state() != TaskBase::TaskState::FINISHED){
           result_.invalidate_result();
       }
    }

    return result_;
}

}

/// \brief Apply Body on the elements of Range
/// The application of the Body on Range in handled by the Executor type
/// If this operation blocks or not depends on????
template<typename Range, typename Body, typename Executor>
ResultHolder<void>
parallel_for(Range& range, const Body& op, Executor& executor){

    if(!range.has_partitions()){
        throw InvalidPartitionedObject("The given range does not have partitions");
    }

    if(range.n_partitions() != executor.n_processing_elements()){
        throw InvalidPartitionedObject("Invalid number of partitions: "+std::to_string(range.n_partitions())+" should be: "+std::to_string(executor.n_processing_elements()));
    }

    detail::parallel_for<Range, Body> algo(range, op);
    ResultHolder<void> result = algo.execute(executor);
    return result;
}
}

#endif // PARALLEL_FOR_H
