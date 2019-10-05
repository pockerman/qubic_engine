#ifndef PARALLEL_FOR_H
#define PARALLEL_FOR_H

#include "kernel/base/config.h"
#include "kernel/base/exceptions.h"

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

    /// \brief Constructor
    parallel_for(RangeTp& range, BodyTp& body);


    /// \brief Execute the parallel_for algorithm using the given executor
    template<typename ExecutorTp>
    void execute(ExecutorTp& executor);

private:

    /// \brief The range over which the algorithm works
    range_type& range_;


    /// \brief The operation applied on the range elements
    body_type& body_;
};


template<typename RangeTp, typename BodyTp>
parallel_for<RangeTp, BodyTp>::parallel_for(RangeTp& range, BodyTp& body)
    :
range_(range),
body_(body)
{}


/// \brief Execute the parallel_for algorithm using the given executor

template<typename RangeTp, typename BodyTp>
template<typename ExecutorTp>
void
parallel_for<RangeTp, BodyTp>::execute(ExecutorTp& executor){

}

}

/// \brief Apply Body on the elements of Range
/// The application of the Body on Range in handled by the Executor type
/// If this operation blocks or not depends on????
template<typename Range, typename Body, typename Executor>
void parallel_for(Range& range, const Body& op, Executor& executor){

    if(!range.has_partitions()){
        throw InvalidPartitionedObject("The given range does not have partitions");
    }

    if(range.n_partitions() != executor.n_processing_elements()){
        throw InvalidPartitionedObject("Invalid number of partitions: "+std::to_string(range.n_partitions())+" should be: "+std::to_string(executor.n_processing_elements()));
    }

    detail::parallel_for<Range, Body> algo(range, op);
    algo.execute(executor);

}
}

#endif // PARALLEL_FOR_H
