#include "parframe/data_structs/range_1d.h"
#define BOOST_TEST_MODULE RANGE_1D_TEST

//unit testing from boost
#include <boost/test/unit_test.hpp>
#include <vector>
#include <utility>

BOOST_AUTO_TEST_CASE (RANGE_1D_DEFAULT_CTOR_TEST)
{
		/*
		 * Test default construction
		 */
			
		using namespace parframe;
		range1d<uint_t> range;
}

BOOST_AUTO_TEST_CASE (RANGE_1D_ARGS_CTOR_TEST)
{
		/*
		 * Test default construction
		 */
			
		using namespace parframe;
		range1d<uint_t> range(0, 10);

		//we should get size 10
    BOOST_CHECK(range.size() == 10);
}
