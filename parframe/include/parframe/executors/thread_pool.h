#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "parframe/base/types.h"
#include "parframe/base/kernel_consts.h"

#include <boost/core/noncopyable.hpp>

#include <memory>
#include <vector>

namespace parframe
{

class TaskBase;

namespace detail{

class parframe_thread;

}

class ThreadPool: private boost::noncopyable
{
public:

    /// \brief Constructor. Initialize the pool with the given number
    /// of threads
    ThreadPool(uint_t n_threads);

    /// Destructor
    ~ThreadPool();

    /// \brief Terminates the threads in the pool.
    /// For every thread stop() and join() is called
    void close();

    /// \brief Allocate the task for execution to one of the
    /// worker threads
    void add_task(TaskBase& task);

    /// \brief Returns the number of threads the pool is using
    uint_t get_n_threads()const{return pool_.size();}

private:

    typedef detail::parframe_thread thread_type;
    typedef std::vector<std::unique_ptr<thread_type>> pool_t;

    /// \brief The pool of workers
    pool_t pool_;
    uint_t next_thread_available_ {parframe::kernel_consts::invalid_size_type()};
};

}

#endif // THREAD_POOL_H
