#ifndef SIMPLE_TASK_H
#define SIMPLE_TASK_H

#include "parframe/executors/task_base.h"
#include "parframe/base/kernel_consts.h"

namespace parframe
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
    SimpleTaskBase(uint_t id = kernel::kernel_consts::invalid_size_type());
};


}

#endif // SIMPLE_TASK_H
