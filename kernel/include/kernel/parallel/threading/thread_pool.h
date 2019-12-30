#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/parallel/threading/task_uitilities.h"

#include <boost/core/noncopyable.hpp>

#include <memory>
#include <vector>
#include <thread>

namespace kernel
{

class TaskBase;

namespace detail{

class kernel_thread;

}

/**
 * @brief The ThreadPool class. Executes tasks using C++11 threading
 */
class ThreadPool: private boost::noncopyable
{
public:

    /// \brief Type of task this pool handles
    typedef TaskBase task_t;

    /// \brief Constructor. Initialize the pool with the given number
    /// of threads
    ThreadPool(uint_t n_threads, bool start_=true);

    /// Destructor
    ~ThreadPool();

    /// \brief Start the thread pool
    void start();

    /// \brief Terminates the threads in the pool.
    /// For every thread stop() and join() is called
    void close();

    /// \brief Allocate the task for execution to one of the
    /// worker threads
    void add_task(TaskBase& task);

    /// \brief Allocate the given tasks for execution
    void add_tasks(const std::vector<std::unique_ptr<TaskBase>>& tasks);

    /// \brief Execute the tasks with the given options
    /// Options aregument currently has no effect
    template<typename TaskTypePtr, typename Options>
    void execute(const std::vector<std::unique_ptr<TaskTypePtr>>& tasks, const Options& options = Null() );

    /// \brief Returns the number of threads the pool is using
    uint_t get_n_threads()const{return pool_.size();}

    /// \brief Returns the number of threads
    uint_t n_processing_elements()const{return get_n_threads();}

private:

    typedef detail::kernel_thread thread_type;
    typedef std::vector<std::unique_ptr<thread_type>> pool_t;

    /// \brief The pool of workers
    pool_t pool_;
    uint_t n_threads_;
    uint_t next_thread_available_ {kernel::KernelConsts::invalid_size_type()};
};


template<typename TaskTypePtr, typename Options>
void
ThreadPool::execute(const std::vector<std::unique_ptr<TaskTypePtr>>& tasks, const Options& /*options*/ ){

    if(tasks.empty()){
        return;
    }

    for(uint_t t=0; t<tasks.size(); ++t){
        add_task(*(tasks[t].get()));
    }

    // if the tasks have not finished yet
    // then the calling thread waits here
    while(!taskutils::tasks_finished(tasks)){
       std::this_thread::yield();
    }
}

}

#endif // THREAD_POOL_H
