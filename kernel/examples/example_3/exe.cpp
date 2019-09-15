/**
  * Show case for read-write lock implementation
  * TODO: Review/finalize implementation
  **/

#include "kernel/base/types.h"
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/parallel/threading/read_write_lock.h"

#include <thread>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace  {

using kernel::uint_t;
using kernel::RWLock;

RWLock rw_lock;

class Task1: public kernel::SimpleTaskBase
{
public:

    Task1()
        :
    kernel::SimpleTaskBase()
    {}

protected:

    virtual void run()final;
};

void
Task1::run(){

    uint_t counter{0};
    rw_lock.read_lock();
    while(true){
        counter++;

        if(counter > 100){
            counter = 0;
        }
    }
}

class Task2: public kernel::SimpleTaskBase
{
public:

    Task2()
        :
    kernel::SimpleTaskBase()
    {}

protected:

    virtual void run()final;
};

void
Task2::run(){

    uint_t counter{0};
    rw_lock.read_lock();
    std::cout<<"Thread did not have to wait..."<<std::endl;
    counter++;
}

}

int main(){

    using kernel::ThreadPool;

    //create a pool and start it with two threads
    ThreadPool pool(2);

    Task1 task_1;
    pool.add_task(task_1);
    Task2 task_2;

    //simulate some long computaion
    {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(2s);
    }

    pool.add_task(task_2);

    //this should block
    pool.close();

    //... should have two reads
    if(rw_lock.num_reads() != 2)
        throw std::runtime_error("Incorrect number of reads");

    return 0;
}
