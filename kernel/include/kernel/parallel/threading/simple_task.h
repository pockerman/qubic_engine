#ifndef SIMPLE_TASK_H
#define SIMPLE_TASK_H

#include "kernel/parallel/threading/task_base.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/base/types.h"
#include "kernel/parallel/utilities/result_holder.h"

namespace kernel
{

template<typename ResultType=Null>
class SimpleTaskBase: public TaskBase
{

public:

    /// \brief The result this task returns
    typedef ResultType result_t;

    /// \brief Destructor
    virtual ~SimpleTaskBase();

    /// \brief Execute the task
    void operator()();

    /// \brief Returns the result computed by the task
    virtual ResultHolder<result_t> get_result()const{return result_; }

protected:

    /// \brief Constructor
    SimpleTaskBase(uint_t id = kernel::KernelConsts::invalid_size_type());

    /// \brief The result computed by the task
    ResultHolder<result_t> result_;

};

template<typename ResultType>
SimpleTaskBase<ResultType>::SimpleTaskBase(uint_t id)
    :
   TaskBase(id)
{
    this->set_name("SimpleTaskBase");
}

template<typename ResultType>
SimpleTaskBase<ResultType>::~SimpleTaskBase()
{}

template<typename ResultType>
void
SimpleTaskBase<ResultType>::operator()(){

    try {

        // assume that the task state may have been set and
        // be different than PENDING in this case we don't run
        if(get_state() != TaskBase::TaskState::PENDING){
            return;
        }

        set_state(TaskBase::TaskState::STARTED);
        run();
        set_state(TaskBase::TaskState::FINISHED);
    }
    catch (...) {

        //whatever caused this, we assume that the task was interrupted
        set_state(TaskBase::TaskState::INTERRUPTED);
    }
}


}

#endif // SIMPLE_TASK_H
