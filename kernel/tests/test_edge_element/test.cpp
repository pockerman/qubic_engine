#include "kernel/base/types.h"
#include "kernel/discretization/edge_element.h"

#include <vector>
#include <gtest/gtest.h>

namespace{

}

TEST(TestEdgeElem, TestInvalidNeighborIdx) {

    /***
       * Test Scenario:    The application attempts to access a neighbor with invalid neighbor index
       * Expected Output:  std::logic_error should be thrown
     **/

    using kernel::uint_t;
    using kernel::numerics::EdgeElem;
    EdgeElem<1> elem(0, 0);

    uint_t n = 3;
    try {
        elem.set_neighbor(n, nullptr);
    }
    catch (std::logic_error& e) {

        const std::string expected("Invalid neighbor index "+std::to_string(n)+" not in [0, "+std::to_string(elem.n_faces())+")");
        ASSERT_EQ(e.what(), expected);
    }

}


TEST(TestEdgeElem, TestReserveZeroNeighbors) {

    /***
       * Test Scenario:   The application attempts to reserve neighbors but uses incorrect number
       * Expected Output: std::logic_error should be thrown
     **/

    using kernel::uint_t;
    using kernel::numerics::EdgeElem;
    EdgeElem<1> elem(0, 0);

    uint_t n = 0;
    try {
        elem.reserve_neighbors(n);
    }
    catch (std::logic_error& e) {

        const std::string expected("Invalid neighbors number "+std::to_string(n)+" not equal to "+std::to_string(elem.n_faces()));
        ASSERT_EQ(e.what(), expected);
    }

}

TEST(TestEdgeElem, TestInsertNeighbor) {

    /***
       * Test Scenario:   The application attempts to access a neighbor with valid neighbor index
       * Expected Output: Neighbor should be returned
     **/

    using kernel::uint_t;
    using kernel::numerics::EdgeElem;
    EdgeElem<1> elem(0, 0);

    uint_t n = 2;
    elem.reserve_neighbors(n);

    std::shared_ptr<EdgeElem<1>> neigh1 = std::make_shared<EdgeElem<1>>(1, 0);
    elem.set_neighbor(0, neigh1.get());

    std::shared_ptr<EdgeElem<1>> neigh2 = std::make_shared<EdgeElem<1>>(2, 0);
    elem.set_neighbor(1, neigh2.get());

    ASSERT_EQ(elem.neighbor_ptr(0)->get_id(), neigh1->get_id());
    ASSERT_EQ(elem.neighbor_ptr(1)->get_id(), neigh2->get_id());

}

TEST(TestEdgeElem, TestEmptyNeighborsList) {

    /***
       * Test Scenario:   The application attempts to access a neighbor with valid neighbor index
       *                   but neighbors list is empty
       * Expected Output: std::logic_error should be thrown
     **/

    using kernel::uint_t;
    using kernel::numerics::EdgeElem;
    EdgeElem<1> elem(0, 0);

    try {
        elem.get_neighbor(0);
    }
    catch (std::logic_error& e) {
        const std::string expected("Neighbors list have not been initialized");
        ASSERT_EQ(e.what(), expected);

    }
}

TEST(TestEdgeElem, TestFullNeighborsListInvlaidIdx) {

    /***
       * Test Scenario:   The application attempts to access a neighbor with valid an invalid neighbor index
       *
       * Expected Output: std::logic_error should be thrown
     **/

    using kernel::uint_t;
    using kernel::numerics::EdgeElem;
    EdgeElem<1> elem(0, 0);

    uint_t n = 3;
    try {
        elem.get_neighbor(n);
    }
    catch (std::logic_error& e) {
        const std::string expected("Invalid neighbor index "+std::to_string(n)+" not in [0, "+std::to_string(elem.n_faces())+")");
        ASSERT_EQ(e.what(), expected);

    }
}




