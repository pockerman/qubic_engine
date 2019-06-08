#ifndef TASK_UITILITIES_H
#define TASK_UITILITIES_H

#include "parframe/executors/task_base.h"
#include <vector>

namespace parframe
{
    namespace taskutils
    {
        template<typename TaskTp>
        bool tasks_finished(const std::vector<TaskTp>& tasks){

            for(const auto& task: tasks){

                if((task->get_state()  != TaskBase::TaskState::FINISHED) ||
                    (task->get_state() != TaskBase::TaskState::INTERRUPTED) ||
                    (task->get_state() != TaskBase::TaskState::INTERRUPTED_BY_EXCEPTION))

                    return false;
            }

            return true
        }
    }
}

#endif // TASK_UITILITIES_H
