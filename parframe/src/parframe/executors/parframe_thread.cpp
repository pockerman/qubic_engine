#include "parframe/executors/parframe_thread.h"
#include "parframe/executors/task_base.h"

namespace parframe
{

namespace detail
{

parframe_thread::parframe_thread(uint_t id)
    :
      working_(false),
      started_(false),
      stop_(false),
      id_(id),
      t_(),
      tasks_()
 {}

void
parframe_thread::do_work_(){

  typedef parframe_thread::task_type_ptr task_type_ptr;

  //if the thread has not been told to stop
  //the try to get some work to do
  while(!stop_){

      task_type_ptr task = nullptr;

      if(tasks_.pop(task)){

              working_ = true;

              //the task may have children
              //in order to respect locality of data
              //execute the returned child
              //while(task != nullptr){
              //    task = task->execute();
              //}
              (*task)();
              working_ = false;
      }
      else{

              working_ = false;
              std::this_thread::yield();
      }
  }
}


void
parframe_thread::start(){

    //if the thread is already running then
    //there is nothing to do
     if(started_){
        return;
     }

     t_ = std::thread(&parframe_thread::do_work_,this);
     started_ = true;
}


void
parframe_thread::stop(){

    if(stop_ == true){
      return;
    }

    //tell the thread first to stop
    stop_ = true;

    join();

    started_ = false;
    working_ = false;
}

void
parframe_thread::push_task(parframe_thread::task_type& task){
    tasks_.push_task(&task);
}

void
parframe_thread::join(){

    if(joinable())
        t_.join();
}

}//detail

}//parframe
