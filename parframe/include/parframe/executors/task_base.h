#ifndef TASK_BASE_H
#define TASK_BASE_H

#include <boost/noncopyable.hpp>

namespace parframe
{

/// \brief Base class for task execution. A task cannot be
/// copied not copy assigned. It can only be moved
class TaskBase: boost::noncopyable
{

public:

    /// \brief A useful enumeration describing the state at which the
    /// task is at. Upon construction task is at PENDING state.
    enum class TaskState{PENDING, STARTED, INTERRUPTED, INTERRUPTED_BY_EXCEPTION, FINISHED, UNDEFINED};

    /// \brief Destructor
    virtual ~TaskBase();

    /// \brief Set the state of the task
    void set_state(TaskState state){state_ = state;}

    /// \brief Query the state of the task
    TaskState get_state()const{return state_;}

    /// \brief Execute the task
    virtual void operator()();

    /// \brief Returns true if the task has completed
    bool finished()const;

    /// \brief Returns true if the taks has children
    bool has_children()const{return false;}

protected:

    /// \brief Constructor
    TaskBase();

    /// \brief Function to overrided by defived classes.
    /// It actually executes the compuational task
    virtual void run()=0;

    /// \brief The state of the task. Upon creation the
    /// state is TaskState::PENDING
    TaskState state_;

};

}

#endif // TASK_BASE_H
