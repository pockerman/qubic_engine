#include "cubic_engine/maths/array_stats.h"
#include "cubic_engine/base/cubic_engine_types.h"

#include <vector>
#include <gtest/gtest.h>

namespace test_data
{

}

/**
 * @brief TEST PearsonCorrelationSuccess
 * Test Scenario:  Calculat the Pearson correlation between two valid arrays
 * Expected Output: Coefficient should be computed
 */

TEST(TestArrayStats, PearsonCorrelationSuccess) {

    using namespace cengine;

    std::vector<real_t> data(20, 1.0);

    auto rslt = pearson_corr(data, data);

    ASSERT_EQ(rslt, 1.0)<<"TestArrayStats::PearsonCorrelationSuccess failed\n";

}
