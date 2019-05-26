#include "parframe/executors/stoppable_task.h"
#include "parframe/base/types.h"
#include <vector>
#include <gtest/gtest.h>

namespace{

    struct TestCondition{

        bool condition{false};
        bool stop()const{return condition;}
        TestCondition(bool cond)
            :
            condition(cond)
        {}
    };

    class TestTask: public parframe::StoppableTask<TestCondition>
    {
    public:

        TestTask(const TestCondition& cond);

        virtual void run()final;

    };

    TestTask::TestTask(const TestCondition& cond)
        :
          parframe::StoppableTask<TestCondition> (cond)
    {}

    void
    TestTask::run(){

        parframe::uint_t counter = 0;
        while((!this->should_stop() && counter < 10)){

                counter++;
        }
    }
}

TEST(TestStoppableTask, TaskFinished) {

    /***
     * Test Scenario: The application creates a task. The task executs and  finishes with FINISHED state
     * Expected Output: Task state should be parframe::TaskBase::TaskState::FINISHED upon task completion
     **/

    // The task to execute
    TestTask t_task(TestCondition(false));

    // Run the task
    t_task();

    ASSERT_EQ(t_task.get_state(), parframe::TaskBase::TaskState::FINISHED);
}

TEST(TestStoppableTask, TaskStopped) {

    /***
     * Test Scenario: The application creates a task and interrupts it
     * Expected Output:  Task state should be parframe::TaskBase::TaskState::INTERRUPTED state
     **/

    // The task to execute
    TestTask t_task(TestCondition(true));

    // Run the task
    t_task();

    ASSERT_EQ(t_task.get_state(), parframe::TaskBase::TaskState::INTERRUPTED);
}


