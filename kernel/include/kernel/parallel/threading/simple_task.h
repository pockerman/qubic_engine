#ifndef SIMPLE_TASK_H
#define SIMPLE_TASK_H

#include "kernel/parallel/threading/task_base.h"
#include "kernel/base/kernel_consts.h"

namespace kernel
{

class SimpleTaskBase: public TaskBase
{

public:

    /// \brief Destructor
    virtual ~SimpleTaskBase();

    /// \brief Execute the task
    void operator()();

protected:

    /// \brief Constructor
    SimpleTaskBase(uint_t id = kernel::KernelConsts::invalid_size_type());
};


}

#endif // SIMPLE_TASK_H
