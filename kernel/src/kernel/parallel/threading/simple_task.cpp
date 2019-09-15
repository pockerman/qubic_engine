#include "kernel/parallel/threading/simple_task.h"

namespace kernel
{

SimpleTaskBase::SimpleTaskBase(uint_t id)
    :
   TaskBase(id)
{}

SimpleTaskBase::~SimpleTaskBase()
{}

void
SimpleTaskBase::operator()(){

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
