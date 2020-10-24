#include "cubic_engine/estimation/particle_filter.h"

#include <vector>
#include <gtest/gtest.h>

TEST(TestParticleFilter, set_weights) {

    /***
     * Test that set_weight does not set the weights
     * if the weights vector does not have the right number of weights
     **/

    using namespace cengine;

    /// initialize the filter with two weights
    ParticleFilter filter(static_cast<uint_t>(2), static_cast<real_t>(2));
    std::vector<real_t> weights(3, 0.0);
    EXPECT_ANY_THROW(filter.set_weights(weights));
}
