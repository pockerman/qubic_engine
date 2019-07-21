#include "cubic_engine/search/a_star_search.h"
#include "cubic_engine/base/cubic_engine_types.h"
#include "parframe/data_structs/boost_serial_graph.h"
#include "parframe/maths/lp_metric.h"

#include <vector>
#include <gtest/gtest.h>

namespace test_data
{
    using real_t = cengine::real_t;
    using unit_t = cengine::uint_t;
    using Vec = cengine::DynVec<real_t>;

    //vertex data to apply A*
    struct astar_node
    {
        real_t gcost;
        real_t fcost;
        Vec position;

        astar_node();
        astar_node(const Vec& p);
        astar_node(const astar_node& o);

    };

    astar_node::astar_node()
    :
    gcost(std::numeric_limits<real_t>::max()),
    fcost(std::numeric_limits<real_t>::max()),
    position()
    {}

    astar_node::astar_node(const Vec& p)
    :
    gcost(std::numeric_limits<real_t>::max()),
    fcost(std::numeric_limits<real_t>::max()),
    position(p)
    {}

    astar_node::astar_node(const astar_node& o)
    :
    gcost(o.gcost),
    fcost(o.fcost),
    position(o.position)
    {}
}

/**
 * @brief TEST Pathfinding with A*
 * Scenario Start and goal nodes are the same
 * Expected Output: Return start node id
 */

TEST(TestAStar, StarAndEndNodesAreTheSame) {

    using namespace cengine;

    using test_data::astar_node;
    kernel::BoostSerialGraph<astar_node, void > graph;
    DynVec<real_t> vec(2, 0.0);

    auto v = graph.add_vertex(std::move(astar_node(vec)));

    kernel::EuclideanMetric metric;
    std::multimap<uint_t, uint_t> path = astar_search(graph, graph.get_vertex(0), graph.get_vertex(0), metric);

    ASSERT_EQ(path.size(), static_cast<uint_t>(1));
    auto start = path.begin();
    ASSERT_EQ(start->first, static_cast<uint_t>(0));
    ASSERT_EQ(start->second, static_cast<uint_t>(0));
}

/**
 * @brief TEST Pathfinding with A*
 * Scenario: Construct a linear path with 5 vertices that is represented as a graph.
 * Expected Output: The reconstructed path should simply be 0,1,2,3,4
 */

TEST(TestAStar, LinearPath) {

    using namespace cengine;

    using test_data::astar_node;
    kernel::BoostSerialGraph<astar_node, void > graph;
    DynVec<real_t> vec(3, 0.0);

    graph.add_vertex(std::move(astar_node(vec)));
    graph.add_vertex(std::move(astar_node(DynVec<real_t>({1.0, 0.0, 0.0 }))));
    graph.add_vertex(std::move(astar_node(DynVec<real_t>({2.0, 0.0, 0.0 }))));
    graph.add_vertex(std::move(astar_node(DynVec<real_t>({3.0, 0.0, 0.0 }))));
    graph.add_vertex(std::move(astar_node(DynVec<real_t>({4.0, 0.0, 0.0 }))));

    graph.add_edge(0,1);
    graph.add_edge(1,2);
    graph.add_edge(2,3);
    graph.add_edge(3,4);

    kernel::EuclideanMetric metric;
    std::multimap<uint_t, uint_t> path = astar_search(graph, graph.get_vertex(0), graph.get_vertex(4), metric);
    std::vector<uint_t> waypoints = cengine::reconstruct_a_star_path(path,graph.get_vertex(4).id);

    ASSERT_EQ(path.empty(), false);
    ASSERT_EQ(path.size(), static_cast<uint_t>(1));
    auto start = path.begin();
    ASSERT_EQ(start->first, static_cast<uint_t>(0));
    ASSERT_EQ(start->second, static_cast<uint_t>(0));

    ASSERT_EQ(waypoints.empty(), false);
    ASSERT_EQ(waypoints.size(), 5);

    //the reversed path should be 0->1->2->3->4
    ASSERT_EQ(waypoints[0], 0);
    ASSERT_EQ(waypoints[1], 1);
    ASSERT_EQ(waypoints[2], 2);
    ASSERT_EQ(waypoints[3], 3);
    ASSERT_EQ(waypoints[4], 4);
}

/**
 * \brief TEST Pathfinding with A*:
 * Scenario: We construct a  path with 7 vertices that is represented as a graph.
 * Expected Output: The reconstructed path should simply be 0->2->5->6->7
 */
TEST(TestAStar, SearchPath) {

    using namespace cengine;

    using test_data::astar_node;
    kernel::BoostSerialGraph<astar_node, void > graph;
    DynVec<real_t> vec(3, 0.0);

    graph.add_vertex(std::move(astar_node(vec)));
    graph.add_vertex(std::move(astar_node(DynVec<real_t>({0.5, -0.5, 0.0}))));
    graph.add_vertex(std::move(astar_node(DynVec<real_t>({1.0,  1.0,  0.0}))));
    graph.add_vertex(std::move(astar_node(DynVec<real_t>({1.5,  -1.0, 0.0}))));
    graph.add_vertex(std::move(astar_node(DynVec<real_t>({2.0,   0.0,  0.0}))));
    graph.add_vertex(std::move(astar_node(DynVec<real_t>({2.0,   1.0,  0.0}))));
    graph.add_vertex(std::move(astar_node(DynVec<real_t>({3.0,   0.5,  0.0}))));
    graph.add_vertex(std::move(astar_node(DynVec<real_t>({4.0, 0.5,  0.0}))));
    graph.add_edge(0,1);
    graph.add_edge(0,2);
    graph.add_edge(2,4);
    graph.add_edge(2,5);
    graph.add_edge(4,6);
    graph.add_edge(5,6);
    graph.add_edge(6,7);
    graph.add_edge(1,3);
    graph.add_edge(3,7);

    kernel::EuclideanMetric metric;
    std::multimap<uint_t, uint_t> path = astar_search(graph, graph.get_vertex(0), graph.get_vertex(7), metric);
    std::vector<uint_t> waypoints = cengine::reconstruct_a_star_path(path,graph.get_vertex(7).id);

    ASSERT_EQ(path.empty(), false);
    ASSERT_EQ(path.size(), static_cast<uint_t>(5));
    auto start = path.begin();
    ASSERT_EQ(start->first, static_cast<uint_t>(0));
    ASSERT_EQ(start->second, static_cast<uint_t>(0));

    ASSERT_EQ(waypoints.empty(), false);
    ASSERT_EQ(waypoints.size(), 5);

    //the reversed path should be 0->2->5->6->7
    ASSERT_EQ(waypoints[0], 0);
    ASSERT_EQ(waypoints[1], 2);
    ASSERT_EQ(waypoints[2], 5);
    ASSERT_EQ(waypoints[3], 6);
    ASSERT_EQ(waypoints[4], 7);
}
