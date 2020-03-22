#include "cubic_engine/control/pure_pursuit_path_tracker.h"
#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/discretization/line_mesh.h"
#include "kernel/discretization/edge_element.h"
#include "kernel/discretization/node.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/dynamics/system_state.h"


#include <vector>
#include <string>
#include <array>
#include <gtest/gtest.h>

namespace
{
    using real_t = cengine::real_t;
    using unit_t = cengine::uint_t;
    using Vec = cengine::DynVec<real_t>;
    using Node = kernel::numerics::Node<2>;
    using GeomPoint = kernel::GeomPoint<2>;
    using LineMesh = kernel::numerics::LineMesh<2>;
    using Edge = kernel::numerics::EdgeElem<2>;
    using PurePersuitTracker = cengine::control::PurePursuit2DPathTracker;
    using SysState = kernel::dynamics::SysState<3>;
}

///
/// \brief TEST
/// Scenario: The robot is located at the left of the path and thus it needs to
///             turn right
/// Expected Output:  Positive curvature sign
///

TEST(TestPurePersuitTracker, RobotPositionAtTheLeft) {

    using namespace cengine;

    LineMesh mesh;
    mesh.reserve_nodes(2);
    mesh.reserve_elements(1);

    auto node0 = mesh.add_node(GeomPoint({0.0, 0.0}));
    auto node1 = mesh.add_node(GeomPoint({0.0, 1.0}));
    auto elem = mesh.add_element();
    elem->resize_nodes();
    elem->set_node(0, node0);
    elem->set_node(1, node1);

    PurePersuitTracker tracker;
    tracker.update(mesh);
    tracker.set_n_sampling_points(4);
    tracker.set_lookahead_dist(1.);

    std::array<std::string, 3> names = {"X","Y","Theta"};
    SysState state(std::move(names), 0.0);
    state.set("X", -1.0);
    state.set("Y", 0.5);


    auto [point, curvature] = tracker.execute(state);

    ASSERT_TRUE(curvature > 0);
}

///
/// \brief TEST
/// Scenario: The robot is located at the right of the path and thus it needs to
///             turn left
/// Expected Output:  Negative curvature sign
///
TEST(TestPurePersuitTracker, RobotPositionAtTheRight) {

 /*

        using namespace cengine;

        LineMesh mesh;
        mesh.reserve_nodes(2);
        mesh.reserve_elements(1);


        auto node0 = mesh.add_node(GeomPoint({0.0, 0.0}));
        auto node1 = mesh.add_node(GeomPoint({0.0, 1.0}));
        auto elem = mesh.add_element();
        elem->resize_nodes();
        elem->set_node(0, node0);
        elem->set_node(1, node1);

        PurePersuitTracker tracker;
        tracker.update(mesh);
        tracker.set_n_sampling_points(4);
        tracker.set_lookahead_dist(1.);

        std::array<std::string, 3> names = {"X","Y","Theta"};
        SysState state(std::move(names), 0.0);
        state.set("X", 1.0);
        state.set("Y", 0.5);

        auto [point, curvature] = tracker.execute(state);

        ASSERT_TRUE(curvature < 0);*/

}

///
/// \brief TEST Pathfinding with A*:
/// Scenario: We construct a  path with 7 vertices that is represented as a graph.
/// Expected Output: The reconstructed path should simply be 0->2->5->6->7
///
TEST(TestPurePersuitTracker, SearchPath) {

    using namespace cengine;

    ///
    ///ASSERT_EQ(path.empty(), false);
    ///ASSERT_EQ(path.size(), static_cast<uint_t>(5));
    ///auto start = path.begin();
    ///ASSERT_EQ(start->first, static_cast<uint_t>(0));
    ///ASSERT_EQ(start->second, static_cast<uint_t>(0));
    ///
    ///ASSERT_EQ(waypoints.empty(), false);
    ///ASSERT_EQ(waypoints.size(), 5);
    ///
    /////the reversed path should be 0->2->5->6->7
    ///ASSERT_EQ(waypoints[0], 0);
    ///ASSERT_EQ(waypoints[1], 2);
    ///ASSERT_EQ(waypoints[2], 5);
    ///ASSERT_EQ(waypoints[3], 6);
    ///ASSERT_EQ(waypoints[4], 7);
}
