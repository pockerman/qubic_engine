#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/search/rapidly_exploring_random_tree.h"
#include "kernel/dynamics/system_state.h"

#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <random>
#include <tuple>


namespace test_data
{    
using uint_t = cengine::uint_t;
using real_t = cengine::real_t;
using cengine::DynVec;
using cengine::DynMat;
using cengine::Null;
using cengine::search::RRT;
using cengine::IdentityMatrix;
using kernel::dynamics::SysState;

}

///
/// \brief
/// Scenario: Create an empty RRT
/// Output:   An empty RRT should be returned
///
TEST(TestRRT, TestEmptyRRT){

    using namespace test_data;
    RRT<Null, Null> rrt;

    ASSERT_EQ(rrt.n_edges(), 0);
    ASSERT_EQ(rrt.n_vertices(), 0);
}

/// \brief
/// Scenario: Create an RRT and build it
/// Output:   A fully functional RRT should be returned to the application
TEST(TestRRT, TestBuild){

    using namespace test_data;
    RRT<SysState<2>, DynVec<real_t>> rrt;

    auto idx = 0;
    auto state_selector = [&idx](){

        // the states that the world is composed of
        static SysState<2> states[] = {SysState<2>(),
                                       SysState<2>(),
                                       SysState<2>(),
                                       SysState<2>()};

        states[0].set(0, {"X", 1.0});
        states[0].set(1, {"Y", 0.0});

        states[1].set(0, {"X", 1.5});
        states[1].set(1, {"Y", 0.0});

        states[2].set(0, {"X", 2.0});
        states[2].set(1, {"Y", 0.0});

        states[3].set(0, {"X", 2.5});
        states[3].set(1, {"Y", 0.0});

        return states[idx++];
    };


    // compute the dynamics of the model
    auto dynamics = [](const SysState<2>& s1, const SysState<2>& /*s2*/){
       SysState<2> s;
       s.set(0, {"X", 0.0});
       s.set(1, {"Y", 0.0});
       s["X"] = s1["X"] + 0.5;
       return std::make_tuple(s, DynVec<real_t>(2, 0.0));
    };

    auto metric = [](const SysState<2>& s1, const SysState<2>& s2 ){
        return l2Norm(s1.as_vector()-s2.as_vector());
    };

    // build the rrt
    SysState<2> xinit;
    xinit.set(0, {"X", 0.0});
    xinit.set(1, {"Y", 0.0});
    rrt.build(4, xinit, state_selector, metric, dynamics);


    // we should have 5 vertices
    ASSERT_EQ(rrt.n_vertices(), 5);
    ASSERT_EQ(rrt.n_edges(), 4);
}

/// \brief
/// Scenario: Application instantiates filter without having initialized
///           the sigma points
/// Output:   std::logic_error is thrown
TEST(TestRRT, TestFindNearestNeighbor){

    using namespace test_data;
    RRT<SysState<2>, DynVec<real_t>> rrt;

    auto idx = 0;
    auto state_selector = [&idx](){

        // the states that the world is composed of
        static SysState<2> states[] = {SysState<2>(),
                                       SysState<2>(),
                                       SysState<2>(),
                                       SysState<2>()};

        states[0].set(0, {"X", 1.0});
        states[0].set(1, {"Y", 0.0});

        states[1].set(0, {"X", 1.5});
        states[1].set(1, {"Y", 0.0});

        states[2].set(0, {"X", 2.0});
        states[2].set(1, {"Y", 0.0});

        states[3].set(0, {"X", 2.5});
        states[3].set(1, {"Y", 0.0});

        return states[idx++];
    };

    // compute the dynamics of the model
    auto dynamics = [](const SysState<2>& s1, const SysState<2>& /*s2*/){
       SysState<2> s;
       s.set(0, {"X", 0.0});
       s.set(1, {"Y", 0.0});
       s["X"] = s1["X"] + 0.5;
       return std::make_tuple(s, DynVec<real_t>(2, 0.0));
    };

    auto metric = [](const SysState<2>& s1, const SysState<2>& s2 ){
        return l2Norm(s1.as_vector()-s2.as_vector());
    };

    // build the rrt
    SysState<2> xinit;
    xinit.set(0, {"X", 0.0});
    xinit.set(1, {"Y", 0.0});
    rrt.build(4, xinit, state_selector,  metric, dynamics);

    // we should have 5 vertices
    ASSERT_EQ(rrt.n_vertices(), 5);

    // ... and four edges
    ASSERT_EQ(rrt.n_edges(), 4);

    RRT<SysState<2>, DynVec<real_t>>::node_t node;
    node.data = xinit;

    auto root = rrt.find_nearest_neighbor(node, metric);

    // the root is the nearest neighbor
    // to xinit
    ASSERT_EQ(root.id, 0);
}
