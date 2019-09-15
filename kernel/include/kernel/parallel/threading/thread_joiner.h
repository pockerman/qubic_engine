#ifndef THREAD_JOINER_H
#define THREAD_JOINER_H

#include "boost/noncopyable.hpp"

namespace kernel
{

/// \brief Class ThreadJoiner is responsible for joining the
/// threads passed to it when it goes out of scope.
/// The class implements the RAII pattern. The class
/// cannot be copied nor copy assigned

template<typename ThreadPoolTp>
class ThreadJoiner: private boost::noncopyable
{

public:

    typedef ThreadPoolTp thread_pool_t;

    /// \brief Constructor. Initialize with the thread-pool
    /// to manage
    ThreadJoiner(ThreadPoolTp& pool)
        :
    pool_(pool)
    {}

    /// \brief Constructor. Responsible for joining
    /// the joinable threads.
    ~ThreadJoiner();


    /// \brief Explicitly join the threads. After this operation
    /// the instance of the ThreadJoiner will be useless
    void join();

private:

    thread_pool_t& pool_;
};


template<typename ThreadPoolTp>
ThreadJoiner<ThreadPoolTp>::~ThreadJoiner(){
   join();
}

template<typename ThreadPoolTp>
void
ThreadJoiner<ThreadPoolTp>::join(){

    for(auto*& t:pool_){

        if(t->joinable())
            t->join();
    }
}

}

#endif // THREAD_HOINER_H
