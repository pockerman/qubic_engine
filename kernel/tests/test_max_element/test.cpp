#include "parframe/utilities/max_element.h"
#include "parframe/executors/iterate_task.h"
#include "parframe/data_structs/range_1d.h"
#define BOOST_TEST_MODULE MAX_ELEMENT_MODULE_TEST

//unit testing from boost
#include <boost/test/unit_test.hpp>
#include <vector>
#include <iostream>


BOOST_AUTO_TEST_CASE (MAX_ELEMENT_COMPUTE_TEST)
{
    using namespace parframe;
    std::vector<int> data(10, 0);
    data[6] = 2;

    MaxElement<std::vector<int>::iterator> max;
    IterateTaskWithResult<std::vector<int>::iterator, MaxElement<std::vector<int>::iterator>> task;

    using namespace parframe;
    range1d<std::vector<int>::iterator> range(data.begin(), data.end());
    task.set_range(range);

    //execute the task
    task();

    //we should get 2
    BOOST_CHECK(task.get_result() == 2);
}
