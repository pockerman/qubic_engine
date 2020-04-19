#include "cubic_engine/grids/waypoint_path.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/base/types.h"

#include <vector>
#include <gtest/gtest.h>

TEST(TestWaypointPath, TestAddWaiPoint) {

    using namespace cengine;
    using kernel::Null;
    using kernel::GeomPoint;

    grids::WaypointPath<2,Null, Null> path;
    path.add_node(GeomPoint<2>(0.0), Null());
    ASSERT_EQ(path.n_nodes(), 1);
}

TEST(TestWaypointPath, TestAddSegmentFail) {

    using namespace cengine;
    using kernel::Null;
    using kernel::GeomPoint;

    grids::WaypointPath<2,Null, Null> path;
    ASSERT_THROW(path.add_element(0, 0, Null()), std::logic_error);
}

TEST(TestWaypointPath, TestAddSegmentFail2) {

    using namespace cengine;
    using kernel::Null;
    using kernel::GeomPoint;

    grids::WaypointPath<2,Null, Null> path;
    path.add_node(GeomPoint<2>(0.0), Null());
    path.add_node(GeomPoint<2>(1.0), Null());
    ASSERT_THROW(path.add_element(0, 0, Null()), std::logic_error);
}

TEST(TestWaypointPath, TestAddSegment) {

    using namespace cengine;
    using kernel::Null;
    using kernel::GeomPoint;

    grids::WaypointPath<2,Null, Null> path;
    path.add_node(GeomPoint<2>(0.0), Null());
    path.add_node(GeomPoint<2>(1.0), Null());

    ASSERT_EQ(path.n_nodes(), 2);

    path.add_element(0, 1, Null());
    ASSERT_EQ(path.n_elements(), 1);
}
