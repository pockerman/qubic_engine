# Example 1: Create ```SimpleTask```s

## Contents

* [Overview](#overview) 
* [Include files](#include_files)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)


## <a name="overview"></a> Overview

## <a name="include_files"></a> Include files

```
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/base/types.h"

#include <vector>
#include <iostream>

```

## <a name="m_func"></a> The main function

```

namespace  {

using kernel::uint_t;

class Task: public kernel::SimpleTaskBase<kernel::Null>
{
public:

    Task()
        :
    kernel::SimpleTaskBase<kernel::Null>()
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

innt main(){

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
```

## <a name="results"></a> Results

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exc.cpp</a>




