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
   range_(range),
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

}

#endif // ITERATE_TASK_H
