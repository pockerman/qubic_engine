#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/base/types.h"

#include <vector>
#include <iostream>

namespace  {

using kernel::uint_t;

class Task: public kernel::SimpleTaskBase
{
public:

    Task()
        :
    kernel::SimpleTaskBase()
    {}

protected:

    virtual void run()final;

};

void
Task::run(){

    uint_t counter{0};
    while(true){
        counter++;

        if(counter > 100){
            counter = 0;
        }
    }
}

}

int main(){

    using kernel::ThreadPool;

    //create a pool and start it with two threads
    ThreadPool pool(2);

    Task task_1;
    pool.add_task(task_1);
    Task task_2;
    pool.add_task(task_2);

    //this should block
    pool.close();

    return 0;
}
