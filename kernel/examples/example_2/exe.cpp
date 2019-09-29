#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/threading/stoppable_task.h"
#include "kernel/base/types.h"

#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

namespace  {

using kernel::uint_t;

struct Condition{

    bool condition{false};
    bool stop()const{return condition;}
    void set_condition(bool cond){condition = cond;}
};

class Task: public kernel::StoppableTask<Condition>
{
public:

    Task()
        :
    kernel::StoppableTask<Condition>()
    {}

protected:

    virtual void run()final;
};

void
Task::run(){

    uint_t counter{0};
    while(!this->should_stop()){
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
    auto& condition = task_1.get_condition();
    pool.add_task(task_1); // add the task to execute

    //simulate some long computaion
    {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(2s);
    }

    //allow the main thread to alter the condition
    condition.set_condition(true);

    //this should block
    pool.close();

    return 0;
}
