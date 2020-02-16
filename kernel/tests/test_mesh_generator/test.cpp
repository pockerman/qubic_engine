#include "kernel/base/types.h"
#include "kernel/discretization/line_mesh.h"
#include "kernel/discretization/mesh_generation.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/discretization/edge_element.h"
#include "kernel/discretization/node.h"
#include "kernel/data_structs/boost_serial_graph.h"
#include "kernel/geometry/geom_point.h"

#include <vector>
#include <gtest/gtest.h>

namespace test_data
{
    using real_t = kernel::real_t;
    using unit_t = kernel::uint_t;
    using kernel::GeomPoint;


    //vertex data to apply A*
    struct astar_node
    {
        real_t gcost;
        real_t fcost;
        GeomPoint<2> position;

        astar_node();
        astar_node(const GeomPoint<2>& p);
        astar_node(const astar_node& o);

    };

    astar_node::astar_node()
    :
    gcost(std::numeric_limits<real_t>::max()),
    fcost(std::numeric_limits<real_t>::max()),
    position()
    {}

    astar_node::astar_node(const GeomPoint<2>& p)
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

TEST(TestLineMesh, TestZeroElements) {

    /***
       * Test Scenario:    The application attempts to generate a LineMesh with zero elements
       * Expected Output:  std::logic_error should be thrown
     **/

    using kernel::uint_t;
    using kernel::numerics::LineMesh;
    using kernel::GeomPoint;
    LineMesh<1> mesh;
    uint_t elements = 0;
    GeomPoint<1> start;
    GeomPoint<1> end;

    try {
        kernel::numerics::build_mesh(mesh, elements, start, end);
    }
    catch (std::logic_error& e) {

        const std::string expected("Cannot generate a LineMesh without elements");
        ASSERT_EQ(e.what(), expected);
    }

}


TEST(TestLineMesh, TestEqualEndPoints) {

    /***
       * Test Scenario:   The application attempts to build a LineMesh with the same starting and ending point
       * Expected Output: std::logic_error should be thrown
     **/

    using kernel::uint_t;
    using kernel::numerics::LineMesh;
    using kernel::GeomPoint;
    LineMesh<1> mesh;
    uint_t elements = 1;
    GeomPoint<1> start;
    GeomPoint<1> end;

    try {
        kernel::numerics::build_mesh(mesh, elements, start, end);
    }
    catch (std::logic_error& e) {

        const std::string expected("Cannot generate a LineMesh with the same start and ending points");
        ASSERT_EQ(e.what(), expected);
    }

}

TEST(TestLineMesh, TestMeshGeneration) {

    /***
       * Test Scenario:   The application attempts to build a LineMesh with 10 elements
       * Expected Output: LineMesh should be build
     **/

    using kernel::uint_t;
    using kernel::numerics::LineMesh;
    using kernel::GeomPoint;
    LineMesh<1> mesh;
    uint_t elements = 3;
    GeomPoint<1> start(0.0);
    GeomPoint<1> end(10.0);

    kernel::numerics::build_mesh(mesh, elements, start, end);

    ASSERT_EQ(mesh.n_elements(), elements);

    // test the elemen-element connectivity

    auto* elem = mesh.get_elem(0);

    ASSERT_EQ(elem->neighbor_ptr(0), nullptr);
    ASSERT_EQ(elem->neighbor_ptr(1)->get_id(),  1);

    uint_t e = 1;
    for(; e < mesh.n_elements() - 1; ++e){

        elem = mesh.get_elem(e);
        ASSERT_EQ(elem->neighbor_ptr(0)->get_id(), e - 1);
        ASSERT_EQ(elem->neighbor_ptr(1)->get_id(), e + 1);
    }

    elem = mesh.get_elem(e);
    ASSERT_EQ(elem->neighbor_ptr(0)->get_id(), e - 1);
    ASSERT_EQ(elem->neighbor_ptr(1),  nullptr);

    // test the elemen-node connectivity

    ASSERT_EQ(mesh.n_nodes(), elements + 1);

    elem = mesh.get_elem(0);
    auto* node0 = elem->get_node(0);
    auto* node1 = elem->get_node(1);

    ASSERT_TRUE(node0);
    ASSERT_TRUE(node1);

    ASSERT_EQ(node0->get_id(), 0);
    ASSERT_EQ(node1->get_id(), 1);

    elem = mesh.get_elem(1);
    node0 = elem->get_node(0);
    node1 = elem->get_node(1);

    ASSERT_EQ(node0->get_id(), 1);
    ASSERT_EQ(node1->get_id(), 2);

    elem = mesh.get_elem(2);
    node0 = elem->get_node(0);
    node1 = elem->get_node(1);

    ASSERT_EQ(node0->get_id(), 2);
    ASSERT_EQ(node1->get_id(), 3);

}

TEST(TestLineMesh, TestCreateFromGraph) {

    /***
       * Test Scenario:   The application attempts to create a LineMesh<2> from a BoostSerialGraph
       * Expected Output: The mesh should be created
     **/

    using kernel::uint_t;
    using kernel::numerics::LineMesh;
    using kernel::GeomPoint;
    LineMesh<2> mesh;

    using test_data::astar_node;
    kernel::BoostSerialGraph<astar_node, void > graph;

    graph.add_vertex(astar_node(GeomPoint<2>(0.0)));
    graph.add_vertex(astar_node(GeomPoint<2>(1.0)));
    graph.add_vertex(astar_node(GeomPoint<2>(2.0)));
    graph.add_vertex(astar_node(GeomPoint<2>(3.0)));
    graph.add_vertex(astar_node(GeomPoint<2>(4.0)));

    graph.add_edge(0,1);
    graph.add_edge(1,2);
    graph.add_edge(2,3);
    graph.add_edge(3,4);

    std::vector<uint_t> vertices(5);
    for(uint_t i=0; i<vertices.size(); ++i){
        vertices[i] = i;
    }

    try {
        kernel::numerics::build_mesh(mesh, graph, vertices);
        ASSERT_EQ(mesh.n_nodes(), vertices.size());
        ASSERT_EQ(mesh.n_elements(), vertices.size() - 1);
    }
    catch (std::logic_error& e) {

        const std::string expected("Cannot generate a LineMesh with the same start and ending points");
        ASSERT_EQ(e.what(), expected);
    }
}





