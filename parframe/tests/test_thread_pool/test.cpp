#include "parframe/executors/thread_pool.h"
#include "parframe/base/types.h"
#include <vector>
#include <gtest/gtest.h>

namespace{

}

TEST(TestThreadPool, InitializeThreadPoolWithZeroNumberOfThreads) {

    /***
     * Test Scenario:   The application attempts to launch a thread pool using n_threads = 0 
		 * Expected Output:	ParFrameLib throws a std::invalid_argument exception
     **/

		/// TODO: Is there a better way to do this?
		try{
			parframe::ThreadPool pool(0);
		}
		catch(std::invalid_argument& e){
            std::string except_msg(e.what());
            ASSERT_EQ(except_msg, "Cannot start pool with no threads");
		}    
}




