#include "parframe/executors/thread_pool.h"
#include "parframe/executors/parframe_thread.h"

#include <stdexcept> // for std::invalid_argument
namespace parframe
{

ThreadPool::ThreadPool(uint_t n_threads)
    :
pool_()
{
    // TODO: perhaps we could request the system
    // using std::thread::hardware_concurrency()
    // or having a default number of threads?
    if(n_threads == 0){
        throw std::invalid_argument("Cannot start pool with no threads");
    }

    pool_.reserve(n_threads);
    for(uint_t t=0; t<n_threads; ++t){
        pool_.push_back( std::make_unique<detail::parframe_thread>(t) );
        pool_[t]->start();
    }
}

ThreadPool::~ThreadPool(){
    close();
}


void
ThreadPool::add_task(TaskBase& task){

    if(next_thread_available_ == parframe::kernel_consts::invalid_size_type() ||
       next_thread_available_ >= pool_.size()){
        next_thread_available_=0;
    }

    // pass the task
    pool_[next_thread_available_]->push_task(task);
    next_thread_available_++;
}




inline
void
ThreadPool::close(){

    for(uint_t t=0; t<pool_.size(); ++t){
        if(pool_[t]){
            pool_[t]->stop();
            pool_[t]->join();
        }
    }

    //joiner_.join();
}





}
