#ifndef ITERATE_TASK_H
#define ITERATE_TASK_H

#include "kernel/utilities/range_1d.h"
#include <algorithm>

namespace kernel
{

/// forward declartions
template<typename IteratorTp, typename OpTp> class IterateTaskWithResult;

template<typename IteratorTp, typename OpTp>
class IterateTask
{
   public:

        typedef IteratorTp iterator_t;
        typedef OpTp operation_t;

        /// \brief Default constructor. No work is done
        /// if  a task is created via this function and
        /// not manipulated appropriately by the public API
        /// of this clas
        IterateTask();

        /// \brief Constructor. Initialize with the range
        /// the task is meant to work on and any arguments the
        /// operation requires to execute
        template<typename...Args>
        IterateTask(const kernel::range1d<iterator_t>& range, Args&... args);

        /// \brief Execute the task
        void operator()();

        /// \brief Set the range to work on
        void set_range(const kernel::range1d<iterator_t>& range){ range_ = range;}

        /// \brief Set the operation data
        void set_op(const operation_t& op){ op_ = op;}

private:

    /// \brief The range that the task is meant to
    /// do work on
    kernel::range1d<iterator_t> range_;

    /// \brief The operation to apply on the elements
    /// in the range above
    operation_t op_;

    /// make friends with IterateTaskWithResult
    friend class IterateTaskWithResult<iterator_t, operation_t>;
};

template<typename IteratorTp, typename OpTp>
IterateTask<IteratorTp, OpTp>::IterateTask()
    :
   range_(),
   op_()
{}

template<typename IteratorTp, typename OpTp>
template<typename... Args>
IterateTask<IteratorTp, OpTp>::IterateTask(const kernel::range1d<iterator_t>& range, Args&... args)
    :
  range_(range),
  op_(args...)
{}

template<typename IteratorTp, typename OpTp>
void
IterateTask<IteratorTp, OpTp>::operator()(){

    auto begin = range_.begin();
    auto end   = range_.end();
    while(begin != end){
        op_(*begin++);
    }
}

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

}

#endif // ITERATE_TASK_H
