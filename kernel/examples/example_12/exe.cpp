#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/parallel/data_structs/lockable_queue.h"
#include "kernel/base/types.h"

#include <vector>
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

namespace  {

using kernel::uint_t;
using kernel::LockableQueue;

class ProducerTask: public kernel::SimpleTaskBase
{
public:

    ProducerTask(LockableQueue<uint_t>& queue, uint_t msg_count, std::mutex& m)
        :
    kernel::SimpleTaskBase(),
    queue_(queue),
    msg_count_(msg_count),
    m_(m)
    {
        this->set_name("ProducerTask");
    }

protected:

    virtual void run()final;

private:

    // the queue over which the two threads will communicate
    LockableQueue<uint_t>& queue_;

    // the message count
    uint_t msg_count_;

    // mutex for controlling I/O
    std::mutex& m_;

};

void
ProducerTask::run(){

    //uint_t counter{0};
    while(msg_count_ != 0){

        {
            std::lock_guard<std::mutex> lock(m_);
            std::cout<<this->get_name()<<" adds to queue: "<<msg_count_<<std::endl;
        }
        queue_.push_item(msg_count_);

        // put to sleep before modifying what was sent.
        // this is not the best solution
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
        msg_count_--;
    }
}

class ConsumerTask: public kernel::SimpleTaskBase
{
public:

    ConsumerTask(LockableQueue<uint_t>& queue, uint_t msg_count, std::mutex& m)
        :
      kernel::SimpleTaskBase(),
      queue_(queue),
      msg_count_(msg_count),
      m_(m)
    {
        this->set_name("ConsumerTask");
    }


protected:

    virtual void run()final;

private:

    // the queue over which the two threads will communicate
    LockableQueue<uint_t>& queue_;

    // the message count
    uint_t msg_count_;

    // mutex for controlling I/O
    std::mutex& m_;
};

void
ConsumerTask::run(){

    while(true){

        auto item = queue_.pop_wait();

        {
            std::lock_guard<std::mutex> lock(m_);
            std::cout<<"Consumer thread got: "<<*item<<std::endl;
        }

        std::this_thread::sleep_for (std::chrono::milliseconds(1));
        msg_count_--;
        if(msg_count_ == 0){
            break;
        }
    }

}

}

int main(){

    using kernel::ThreadPool;
    using kernel::LockableQueue;

    // the queue over which the threads communicate
    LockableQueue<uint_t> queue;

    //create a pool and start it with two threads
    ThreadPool pool(2);

    // the mutex to lock
    std::mutex m;


    ProducerTask task_1(queue, 20, m);
    pool.add_task(task_1);

    ConsumerTask task_2(queue, 20, m);
    pool.add_task(task_2);



    //this should block
    pool.close();

    return 0;
}
