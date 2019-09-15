#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include "kernel/base/types.h"
#include <boost/core/noncopyable.hpp>

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace kernel
{

/***
 * A simple implementation of a thread safe queue.
 * The implementation uses simple lock mechanism.
 * The type of the task held is specified by the template argument
 * TODO: Think about copy operations. Do we really want to diable them?
 * TODO: We need to rethink the internal implementation
 **/

template<typename T>
class TaskQueue: private boost::noncopyable
{

 public:

    typedef T value_type;

    /// \brief Default ctor. Construct an empty queue
    TaskQueue();

    /// \breif Destructor
    ~TaskQueue()
    {}

    //pop an element from the queue. The thread
    //that calls this waits until the queue has
    //at least one element
    value_type* pop_wait();

    //pop an element from the queue. Same as above
    bool pop_wait(value_type*& element);

    //pop an element from the queue. If the queue is empty
    //then return nullptr. This function does not cause the
    //calling thread to wait
    value_type* pop();

     //pop an element from the queue. If the queue is empty
    //then return false. This function does not cause the
    //calling thread to wait
    bool pop(value_type*& element);

    //push an element to the queue
    void push_task(value_type& element);

    //push an element to the queue
    void push_task(value_type* element);

    //push the given list of tasks into the thread private
    //task pool
    template<typename C>
    void push_tasks(const C& tasks);

    //assign tasks to the private thread task pool.
    //std::iterator_traits<Iterator>::value_type
    //should resolve to T*
    template<typename Iterator>
    void push_tasks(Iterator begin,Iterator end);

    //get the size of the queue
    uint_t size()const;

    //is the queue empty
    bool empty()const;

 private:

    struct Node
    {
        //the data we hold are tasks
        value_type* task_;

        Node()
            :
           task_(nullptr)
        {}

        Node(value_type* t)
            :
            task_(t)
        {}

        //shallow copy ctor
        Node(const Node& other)
            :
            task_(other.task_)
        {}

        //shallow copy assignement
        Node& operator=(const Node& other){

            if(this==&other){
                return *this;
            }
            this->task_ = other.task_;
            return *this;
        }

        //comparison operator simply checks the memory
        bool operator==(const Node& other)const{

           //memory comparison
           if(task_ == other.task_){
               return true;
           }

            return false;
        }
     };

     std::queue<Node> task_queue_;
     mutable std::mutex mutex_;
     std::condition_variable cond_;

};

template<typename T>
TaskQueue<T>::TaskQueue()
:
task_queue_()
{}

template<typename T>
inline
uint_t
TaskQueue<T>::size()const{
    return static_cast<uint_t>(task_queue_.size());
}

template<typename T>
inline
bool
TaskQueue<T>::empty()const{
    return task_queue_.empty();
}

template<typename T>
inline
T* 
TaskQueue<T>::pop_wait(){

    typedef typename TaskQueue<T>::Node node;
    std::unique_lock<std::mutex> lk(mutex_);

    //tell the thread to wait until the queue has at least one element
    cond_.wait(lk,[this]{ return !task_queue_.empty();});

    node n = task_queue_.front();
    task_queue_.pop();
    return n.task_;
}

template<typename T>
inline
bool
TaskQueue<T>::pop_wait(T*& ele){

    typedef typename TaskQueue<T>::Node node;
    std::unique_lock<std::mutex> lk(mutex_);

    //tell the thread to wait until the queue has at least one element
    cond_.wait(lk,[this]{ return !task_queue_.empty();});

    node n = task_queue_.front();
    task_queue_.pop();
    ele = n.task_;
    return true;
}

template<typename T>
inline
T* 
TaskQueue<T>::pop(){

    typedef typename TaskQueue<T>::Node node;
    std::lock_guard<std::mutex> lk(mutex_);

    if(task_queue_.empty()) return nullptr;

    node n = task_queue_.front();
    task_queue_.pop();
    return n.task_;
}

template<typename T>
inline
bool
TaskQueue<T>::pop(T*& ele){

    typedef typename TaskQueue<T>::Node node;
    std::lock_guard<std::mutex> lk(mutex_);

    if(task_queue_.empty()) return false;

    node n = task_queue_.front();
    task_queue_.pop();
    ele = n.task_;
    return true;
}

template<typename T>
inline
void 
TaskQueue<T>::push_task(T& element){

    typedef typename TaskQueue<T>::Node node;
    std::lock_guard<std::mutex> lk(mutex_);

    task_queue_.push(node(&element));
    cond_.notify_one();
}

template<typename T>
inline
void 
TaskQueue<T>::push_task(T* element){
    
    typedef typename TaskQueue<T>::Node node;
    std::lock_guard<std::mutex> lk(mutex_);

    //node n(element);

    task_queue_.push(node(element));
    cond_.notify_one();
}

template<typename T>
template<typename C>
void 
TaskQueue<T>::push_tasks(const C& tasks){

    if(tasks.empty()){
        return;
    }

    typedef typename TaskQueue<T>::Node node;
    std::lock_guard<std::mutex> lk(mutex_);

    typedef typename C::const_iterator iterator;

    iterator b = tasks.begin();
    iterator e = tasks.end();

    while(b!=e){

      task_queue_.push(node((*b)));
      b++;
    }

    cond_.notify_one();
}

template<typename T>
template<typename Iterator>
void 
TaskQueue<T>::push_tasks(Iterator begin,Iterator end){

    //quick return
    if(std::distance(begin,end)==static_cast<uint_t>(0)){
        return;
    }
  
    typedef typename TaskQueue<T>::Node node;
    std::lock_guard<std::mutex> lk(mutex_);

    while(begin != end){

      task_queue_.push(node((*begin)));
      begin++;
    }

    cond_.notify_one();
}

}//kernelpp
#endif
