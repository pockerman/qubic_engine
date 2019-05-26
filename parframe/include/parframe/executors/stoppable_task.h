/***
 *
 * Implementation of a task that can be stopped
 * Initial implementation is taken from
 * https://thispointer.com/c11-how-to-stop-or-terminate-a-thread/
 *
 *
 **/

#ifndef STOPPABLE_TASK_H
#define STOPPABLE_TASK_H

#include "parframe/executors/task_base.h"

namespace parframe {


/// \brief Base class for a task that can stopped
/// The stop condition is described by the template
/// parameter. It must be default constructed.
///
template<typename StopCondition>
class StoppableTask: public TaskBase
{

public:

    /// \brief The condition type
    typedef StopCondition stop_condition_t;

    /// \brief Destructor
    ~StoppableTask();

    /// \brief Execute the task
    void operator()();

    /// \brief Returns true if the task should stop
    bool should_stop()const{return stop_cond_.stop();}

    /// \brief Access the stop condition object. since the conditionals may change
    /// allow this to be accessed externally.
    stop_condition_t& get_condition(){return stop_cond_;}

protected:

    /// \brief Constructor
    explicit StoppableTask(const stop_condition_t& condition = stop_condition_t());

    /// \brief The object responsible for providing
    /// the information if the task should be stopped or not
    stop_condition_t stop_cond_;
};

template<typename StopCondition>
StoppableTask<StopCondition>::StoppableTask(const stop_condition_t& condition)
    :
    TaskBase(),
    stop_cond_(condition)
{}

template<typename StopCondition>
StoppableTask<StopCondition>::~StoppableTask()
{}

template<typename StopCondition>
void
StoppableTask<StopCondition>::operator()(){

    try {

        this->set_state(TaskBase::TaskState::STARTED);
        this->run();
        if(!stop_cond_.stop()){
            this->set_state(TaskBase::TaskState::FINISHED);
        }
        else{
           this->set_state(TaskBase::TaskState::INTERRUPTED);
        }
    }
    catch (...) {

        // whatever caused this, we assume that the task was interrupted
        // by an exception
        set_state(TaskBase::TaskState::INTERRUPTED_BY_EXCEPTION);
    }
}

}

#endif // STOPPABLE_TASK_H
