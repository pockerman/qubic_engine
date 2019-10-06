#ifndef ITERATE_TASK_H
#define ITERATE_TASK_H

#include "kernel/utilities/range_1d.h"
#include "kernel/parallel/threading/task_base.h"
#include <algorithm>

namespace kernel
{

/// forward declartions
template<typename IteratorTp, typename OpTp> class IterateTaskWithResult;


/**
 * Helper class to be used when an iteration over a range is required.
 * The overriden run() of this class applies BodyType on every item of ValueType
 * that is in the given RangeType
 */
template<typename RangeType, typename BodyType, typename ValueType>
class IterateTask: public TaskBase
{
   public:

        typedef RangeType range_type;
        typedef BodyType body_type;
        typedef ValueType value_type;


        /// \brief Constructor.
        IterateTask(uint_t id, const range_type& range, const body_type& body, value_type& values);

protected:


        /// \brief Function to overrided by defived classes.
        /// It actually executes the compuational task
        virtual void run()override;

private:

    /// \brief The range that the task is meant to do work on
    /// Ope
    range_type range_;

    /// \brief The operation to apply on the elements
    /// in the range above
    body_type op_;


    /// \brief The values container to apply the Body
    value_type& values_;

};

template<typename RangeType, typename BodyType, typename ValueType>
IterateTask<RangeType, BodyType, ValueType>::IterateTask(uint_t id, const range_type& range, const body_type& body, value_type& values)
    :
   TaskBase(id),
   range_(),
   op_(body),
   values_(values)
{}

template<typename RangeType, typename BodyType, typename ValueType>
void
IterateTask<RangeType, BodyType, ValueType>::run(){

    auto begin = range_.begin();
    auto end   = range_.end();

    while(begin != end){
        op_(values_[begin]);
        begin++;
    }
}


/*
template<typename IteratorTp, typename OpTp>
class IterateTaskWithResult
{

public:

    typedef IteratorTp iterator_t;
    typedef OpTp operation_t;
    typedef typename operation_t::result_t result_t;

    /// \brief Default constructor. No work is done
    /// if  a task is created via this function and
    /// not manipulated appropriately by the public API
    /// of this clas
    IterateTaskWithResult();

    /// \brief Constructor. Initialize with the range
    /// the task is meant to work on and any arguments the
    /// operation requires to execute
    template<typename...Args>
    IterateTaskWithResult(const kernel::range1d<iterator_t>& range, Args&... args);

    /// \brief Execute the task
    void operator()(){task_();}

    /// \brief Set the range to work on
    void set_range(const kernel::range1d<iterator_t>& range){task_.set_range(range);}

    /// \brief Set the operation data
    void set_op(const operation_t& op){ task_.set_op(op);}

    /// \brief Get the result of applying the operation over the
    /// given iterator range
    result_t get_result()const{return task_.op_.get_result();}

private:

    IterateTask<iterator_t, operation_t> task_;

};

template<typename IteratorTp, typename OpTp>
IterateTaskWithResult<IteratorTp, OpTp>::IterateTaskWithResult()
    :
   task_()
{}

template<typename IteratorTp, typename OpTp>
template<typename...Args>
IterateTaskWithResult<IteratorTp, OpTp>::IterateTaskWithResult(const kernel::range1d<iterator_t>& range, Args&... args)
    :
 task_(range, args...)
{}

*/

}

#endif // ITERATE_TASK_H
