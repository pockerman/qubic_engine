#include "cubic_engine/search/a_star_search.h"
#include "cubic_engine/base/cubic_engine_types.h"
#include "parframe/data_structs/boost_serial_graph.h"
#include "parframe/maths/lp_metric.h"

#include <vector>
#include <gtest/gtest.h>

//vertex data to apply A*

namespace test_data
{
    using real_t = cengine::real_t;
    using unit_t = cengine::uint_t;
    using Vec = cengine::DynVec<real_t>;

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
