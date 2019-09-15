#ifndef TASK_UITILITIES_H
#define TASK_UITILITIES_H

#include <vector>

namespace kernel
{
    namespace taskutils
    {   

				/// \brief Queries the task in the given task list whether they finished
		    /// execution or not. It always returns true if the given list is empty
        template<typename TaskTp>
        bool tasks_finished(const std::vector<TaskTp>& tasks){

            for(const auto& task: tasks){

                if(!task->finished()){
                    return false;
                }
            }

            return true;
        }
    }
}

#endif // TASK_UITILITIES_H
