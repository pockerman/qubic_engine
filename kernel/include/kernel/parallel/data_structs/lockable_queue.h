#ifndef LOCKABLE_QUEUE_H
#define LOCKABLE_QUEUE_H

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
class LockableQueue: private boost::noncopyable
{

 public:

    typedef T value_t;

    /// \brief Default ctor. Construct an empty queue
    LockableQueue();

    /// \brief Destructor
    ~LockableQueue()
    {}

    //pop an element from the queue. The thread
    //that calls this waits until the queue has
    //at least one element
    value_t pop_wait();

    //pop an element from the queue. Same as above
    bool pop_wait(value_t& element);

    //pop an element from the queue. If the queue is empty
    //then return nullptr. This function does not cause the
    //calling thread to wait
    //value_t pop();

     //pop an element from the queue. If the queue is empty
    //then return false. This function does not cause the
    //calling thread to wait
    bool pop(value_t& element);

    //push an element to the queue
    void push_item(const value_t& element);

    template<typename Iterator>
    void push_items(Iterator begin,Iterator end);

    //get the size of the queue
    uint_t size()const;

    //is the queue empty
    bool empty()const;

 private:

    struct Node
    {
        //the data we hold are tasks
        value_t value_;

        Node()
            :
           value_()
        {}

        Node(const value_t& value)
            :
            value_(value)
        {}

        //shallow copy ctor
        Node(const Node& other)
            :
            value_(other.value_)
        {}

        //shallow copy assignement
        Node& operator=(const Node& other){

            if(this==&other){
                return *this;
            }
            this->value_ = other.value_;
            return *this;
        }


        bool operator==(const Node& other)const{

           //memory comparison
           if(value_ == other.value_){
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
LockableQueue<T>::LockableQueue()
:
task_queue_()
{}

template<typename T>
inline
uint_t
LockableQueue<T>::size()const{
    return static_cast<uint_t>(task_queue_.size());
}

template<typename T>
inline
bool
LockableQueue<T>::empty()const{
    return task_queue_.empty();
}

template<typename T>
inline
T
LockableQueue<T>::pop_wait(){

    typedef typename LockableQueue<T>::Node node;
    std::unique_lock<std::mutex> lk(mutex_);

    //tell the thread to wait until the queue has at least one element
    cond_.wait(lk,[this]{ return !task_queue_.empty();});

    node n = task_queue_.front();
    task_queue_.pop();
    return n.value_;
}

template<typename T>
inline
bool
LockableQueue<T>::pop_wait(value_t& ele){

    typedef typename LockableQueue<T>::Node node;
    std::unique_lock<std::mutex> lk(mutex_);

    //tell the thread to wait until the queue has at least one element
    cond_.wait(lk, [this]{ return !task_queue_.empty();});

    node n = task_queue_.front();
    task_queue_.pop();
    ele = n.value_;
    return true;
}

/*template<typename T>
inline
T
LockableQueue<T>::pop(){

    typedef typename LockableQueue<T>::Node node;
    std::lock_guard<std::mutex> lk(mutex_);

    if(task_queue_.empty()) return nullptr;

    node n = task_queue_.front();
    task_queue_.pop();
    return n.value_;
}*/

template<typename T>
inline
bool
LockableQueue<T>::pop(T& ele){

    typedef typename LockableQueue<T>::Node node;
    std::lock_guard<std::mutex> lk(mutex_);

    if(task_queue_.empty()) return false;

    node n = task_queue_.front();
    task_queue_.pop();
    ele = n.value_;
    return true;
}

template<typename T>
inline
void 
LockableQueue<T>::push_item(const T& element){
    
    typedef typename LockableQueue<T>::Node node;
    std::lock_guard<std::mutex> lk(mutex_);

    //node n(element);

    task_queue_.push(node(element));
    cond_.notify_one();
}



template<typename T>
template<typename Iterator>
void 
LockableQueue<T>::push_items(Iterator begin,Iterator end){

    //quick return
    if(std::distance(begin,end)==static_cast<uint_t>(0)){
        return;
    }
  
    typedef typename LockableQueue<T>::Node node;
    std::lock_guard<std::mutex> lk(mutex_);

    while(begin != end){

      task_queue_.push(node((*begin)));
      begin++;
    }

    cond_.notify_one();
}

}//kernelpp
#endif
