#include "kernel/base/types.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/parallel/utilities/linear_mesh_partitioner.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/mesh.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/discretization/quad_mesh_generation.h"


#include <vector>
#include <gtest/gtest.h>

namespace{

}

TEST(TestLinearMeshPartitioner, TestOnePartition) {

    /***
       * Test Scenario:    The application attempts to partition a quad mesh with only one partition
       * Expected Output:  All elements should be assigned PE equal to zero
     **/

    using kernel::uint_t;
    using kernel::GeomPoint;
    using kernel::numerics::Element;
    using kernel::numerics::Mesh;
    using kernel::numerics::ElementMeshIterator;
    using kernel::numerics::Active;

    Mesh<2> mesh;
    {
        uint_t nx = 2;
        uint_t ny = 2;

        GeomPoint<2> start(0.0);
        GeomPoint<2> end(1.0);

        // generate the mesh
        kernel::numerics::build_quad_mesh(mesh, nx, ny, start, end);
    }

    kernel::numerics::linear_mesh_partition(mesh, 1);

    ElementMeshIterator<Active, Mesh<2>> filter(mesh);
    auto begin = filter.begin();
    auto end = filter.end();

    uint_t expected = 0;
    for(; begin != end; ++begin){
        auto* element = *begin;
        ASSERT_EQ(element->get_pid(), expected);
    }
}

TEST(TestLinearMeshPartitioner, TestManyPartitions) {

    /***
       * Test Scenario:    The application attempts to partition a quad mesh with many partitions
       * Expected Output:  All elements should be assigned PE a distinct partition
     **/

    using kernel::uint_t;
    using kernel::GeomPoint;
    using kernel::numerics::Element;
    using kernel::numerics::Mesh;
    using kernel::numerics::ElementMeshIterator;
    using kernel::numerics::Active;

    Mesh<2> mesh;
    {
        uint_t nx = 2;
        uint_t ny = 2;

        GeomPoint<2> start(0.0);
        GeomPoint<2> end(1.0);

        // generate the mesh
        kernel::numerics::build_quad_mesh(mesh, nx, ny, start, end);
    }


    kernel::numerics::linear_mesh_partition(mesh, 4);

    ElementMeshIterator<Active, Mesh<2>> filter(mesh);
    auto begin = filter.begin();
    auto end = filter.end();

    uint_t expected_parts[]{0, 1, 2, 3};
    for(; begin != end; ++begin){
        auto* element = *begin;
        ASSERT_EQ(element->get_pid(), expected_parts[element->get_id()]);
    }
}

TEST(TestLinearMeshPartitioner, TestManyUnequalPartitions) {

    /***
       * Test Scenario:    The application attempts to partition a quad mesh with many partitions.
       *                    The number of partitions does not divide equally the number of elements
       * Expected Output:  All elements should be assigned PE a distinct partition
     **/

    using kernel::uint_t;
    using kernel::GeomPoint;
    using kernel::numerics::Element;
    using kernel::numerics::Mesh;
    using kernel::numerics::ElementMeshIterator;
    using kernel::numerics::Active;

    Mesh<2> mesh;
    {
        uint_t nx = 5;
        uint_t ny = 5;

        GeomPoint<2> start(0.0);
        GeomPoint<2> end(1.0);

        // generate the mesh
        kernel::numerics::build_quad_mesh(mesh, nx, ny, start, end);
    }


    kernel::numerics::linear_mesh_partition(mesh, 3);

    ElementMeshIterator<Active, Mesh<2>> filter(mesh);
    auto begin = filter.begin();
    auto end = filter.end();

    uint_t expected_counter1 = 8;
    uint_t expected_counter2 = 8;
    uint_t expected_counter3 = 9;

    uint_t counter1 = 0;
    uint_t counter2 = 0;
    uint_t counter3 = 0;

    for(; begin != end; ++begin){
        auto* element = *begin;

        if(element->get_pid() == 0){
            counter1++;
        }

        if(element->get_pid() == 1){
            counter2++;
        }

        if(element->get_pid() == 2){
            counter3++;
        }
    }

    ASSERT_EQ(counter1, expected_counter1);
    ASSERT_EQ(counter2, expected_counter2);
    ASSERT_EQ(counter3, expected_counter3);
}



