#include "parframe/executors/thread_pool.h"
#include "parframe/executors/parframe_thread.h"

#include <stdexcept> // for std::invalid_argument
namespace parframe
{

ThreadPool::ThreadPool(uint_t n_threads, bool start_)
    :
pool_(),
n_threads_(n_threads)
{
    // TODO: perhaps we could request the system
    // using std::thread::hardware_concurrency()
    // or having a default number of threads?
    if(n_threads_ == 0){
        throw std::invalid_argument("Cannot start pool with no threads");
    }

    if(start_){
        start();
    }
}

ThreadPool::~ThreadPool(){
    close();
}


void
ThreadPool::start()
{
    pool_.reserve(n_threads_);
    for(uint_t t=0; t < n_threads_; ++t){
        pool_.push_back( std::make_unique<detail::parframe_thread>(t) );
        pool_[t]->start();
    }

}

void
ThreadPool::add_task(TaskBase& task){

    if(next_thread_available_ == kernel::kernel_consts::invalid_size_type() ||
       next_thread_available_ >= pool_.size()){
        next_thread_available_=0;
    }

    // pass the task
    pool_[next_thread_available_]->push_task(task);
    next_thread_available_++;
}

void
ThreadPool::add_tasks(const std::vector<std::unique_ptr<task_t>>& tasks){

    for(uint_t t=0; t<tasks.size(); ++t){
        add_task(*(tasks[t].get()));
    }
}


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
