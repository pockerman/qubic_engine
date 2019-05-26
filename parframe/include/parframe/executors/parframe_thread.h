#ifndef PARFRAME_THREAD_H
#define PARFRAME_THREAD_H

#include "parframe/base/types.h"
#include "parframe/data_structs/task_queue.h"
#include <boost/core/noncopyable.hpp>

#include <vector>
#include <memory>
#include <thread>
#include <mutex>

namespace parframe
{

/// forward declarations
class TaskBase;

/**
 * items in this namespace should typically not be
 * used by high level applications
 */

namespace detail
{
/***
 *
 * simple wrapper to std::thread so that the working thread
 * can be assigned a pool of tasks to work on
 **/

class parframe_thread: private boost::noncopyable
{
public:

    /**
     * expose the type of task to be submitted to the
     * internal task queue
     */
    typedef TaskBase task_type;
    typedef task_type* task_type_ptr;

    /**
     * ctor construct by passing the id of the thread
     */
    explicit parframe_thread(uint_t id);

    /**
     * dtor wait until the thread finishes
     */
    ~parframe_thread(){stop();}

    /**
      * Start running the thread
      */
    void start();

    /**
     * Stop the thread
     */
    void stop();


    /// \brief  Return true if and only if the thread can be joined
    bool joinable()const noexcept{return t_.joinable();}

    /// \brief Join the thread. It checks if the underlying std::thread is joinable
    void join();

    /**
     * zero based id of the thread
     */
    uint_t id()const noexcept{return id_;}


    /**
     * add a task to the queue of tasks for this
     * thread
     */
    void push_task(task_type& t);

    /**
     * how many tasks the thread has. This is just a glimpse
     * of the tasks that the current thread has
     */
    uint_t n_tasks()const{return tasks_.size();}

private:

    /**
     * flag indicating whether the thread is working
     */
    bool working_;

    /**
     * flag indicating whether the thread has been started
     */
    bool started_;

    /**
     * flag indicating whether the thread has been stopped
     */
    bool stop_;

    /**
     * the zero based index of the thread
     */
    uint_t id_;

    /// \brief The actual thread
    std::thread t_;

    /// \brief The queue of thread tasks
    TaskQueue<task_type> tasks_;

    /// \brief the function that actually does the work
    void do_work_();
};

}//detail
}//parframe

#endif // PARFRAME_THREAD_H
