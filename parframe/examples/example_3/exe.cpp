/**
  * Show case for read-write lock implementation
  * TODO: Review/finalize implementation
  **/

#include "parframe/executors/thread_pool.h"
#include "parframe/executors/simple_task.h"
#include "parframe/base/types.h"
#include "parframe/utilities/read_write_lock.h"

#include <thread>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace  {

using parframe::uint_t;
using parframe::RWLock;

RWLock rw_lock;

class Task1: public parframe::SimpleTaskBase
{
public:

    Task1()
        :
    parframe::SimpleTaskBase()
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

class Task2: public parframe::SimpleTaskBase
{
public:

    Task2()
        :
    parframe::SimpleTaskBase()
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

    using parframe::ThreadPool;

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
