#include "parframe/executors/task_base.h"

namespace parframe
{

TaskBase::TaskBase()
    :
    state_(TaskBase::TaskState::PENDING)
{}

TaskBase::~TaskBase()
{}


}
