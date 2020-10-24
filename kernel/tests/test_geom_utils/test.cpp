#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/geometry/geometry_utils.h"
#include "kernel/geometry/generic_line.h"
#include "kernel/geometry/geom_point.h"

#include <vector>
#include <gtest/gtest.h>


TEST(TestGeomUtils, TestFindPointOnLineAtDeltaDistance_True) {

    kernel::GeomPoint<2> p1;
    kernel::GeomPoint<2> p2({1.0, 0.0});
    kernel::GenericLine<kernel::GeomPoint<2>> line(p1, p2);
    auto [found, point ] = kernel::find_point_on_line_at_delta_distance(line, 0.5,
            10, kernel::KernelConsts::tolerance());

    kernel::real_t h = 1.0/static_cast<kernel::real_t>(12);
    kernel::real_t expected = 0.0;

    while(expected <= 0.5){
     expected += h;
    }
    ASSERT_EQ(found, true);
    ASSERT_NEAR(point[0], expected, 1.0e-5);
    ASSERT_NEAR(point[1], 0.0, 1.0e-5);
}

TEST(TestGeomUtils, TestFindPointOnLineAtDeltaDistance_False) {

    kernel::GeomPoint<2> p1;
    kernel::GeomPoint<2> p2({1.0, 0.0});
    kernel::GenericLine<kernel::GeomPoint<2>> line(p1, p2);
    auto [found, point ] = kernel::find_point_on_line_at_delta_distance(line, 1.2,
            10, kernel::KernelConsts::tolerance());


    ASSERT_EQ(found, false);
}









