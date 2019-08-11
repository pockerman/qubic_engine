#ifndef TASK_BASE_H
#define TASK_BASE_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"

#include <boost/noncopyable.hpp>
#include <string>

namespace kernel
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

    /// \brief Id of task
    uint_t get_id()const{return id_;}

    /// \brief Set the id of the task
    void set_id(uint_t id){id_ = id;}

    /// \brief Returns the name of the task
    const std::string get_name()const{return name_;}

    /// \brief Set the name of the task
    void set_name(const std::string& name){name_ = name;}

protected:

    /// \brief Constructor
    TaskBase(uint_t id = kernel::kernel_consts::invalid_size_type());

    /// \brief Function to overrided by defived classes.
    /// It actually executes the compuational task
    virtual void run()=0;

    /// \brief The state of the task. Upon creation the
    /// state is TaskState::PENDING
    TaskState state_;

    /// \brief The id of the task
    uint_t id_;

    /// \brief The name of the task. This can act as another identifier
    /// for the task
    std::string name_;

};

}

#endif // TASK_BASE_H
