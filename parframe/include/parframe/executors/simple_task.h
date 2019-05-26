#ifndef SIMPLE_TASK_H
#define SIMPLE_TASK_H

#include "parframe/executors/task_base.h"

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
    SimpleTaskBase();
};


}

#endif // SIMPLE_TASK_H
