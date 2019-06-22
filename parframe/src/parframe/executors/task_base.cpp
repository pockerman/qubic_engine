#include "parframe/executors/task_base.h"


namespace parframe
{

TaskBase::TaskBase(uint_t id)
    :
    state_(TaskBase::TaskState::PENDING),
    id_(id)
{}

TaskBase::~TaskBase()
{}

void
TaskBase::operator()(){

    try {

        // assume that the task state may have been set and
        // be different than PENDING in this case we don't run
        // Perhaps is better if this throws an exception
        if(get_state() != TaskBase::TaskState::PENDING)
            return;

        set_state(TaskBase::TaskState::STARTED);
        run();
        set_state(TaskBase::TaskState::FINISHED);
    }
    catch (...) {

        //whatever caused this, we assume that the task was interrupted
        set_state(TaskBase::TaskState::INTERRUPTED);
    }
}


bool
TaskBase::finished()const{

    if(!has_children()){

      return (state_ != TaskBase::TaskState::PENDING &&
              state_ != TaskBase::TaskState::STARTED &&
              state_ != TaskBase::TaskState::INTERRUPTED &&
              state_ != TaskBase::TaskState::INTERRUPTED_BY_EXCEPTION );
    }

    return false;
}

}
