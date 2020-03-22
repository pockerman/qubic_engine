#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/control/pure_pursuit_path_tracker.h"
#include "kernel/discretization/line_mesh.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/discretization/edge_element.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/discretization/node.h"


#include <array>
#include <iostream>


int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::control::PurePursuit2DPathTracker;
    using kernel::numerics::LineMesh;
    using kernel::GeomPoint;
    using kernel::dynamics::SysState;

    try{

        /// the path to follow
        LineMesh<2> path;

        /// create the path
        auto node0 = path.add_node(GeomPoint<2>({0.0, 0.0}));
        auto node1 = path.add_node(GeomPoint<2>({1.0, 1.0}));
        auto node2 = path.add_node(GeomPoint<2>({2.0, 0.0}));

        auto element = path.add_element();
        element->resize_nodes();
        element->set_node(0, node0);
        element->set_node(1, node1);

        element = path.add_element();
        element->resize_nodes();
        element->set_node(0, node1);
        element->set_node(1, node2);

        std::array<std::string, 3> names = {"X","Y","Theta"};
        SysState<3> state(std::move(names), 0.0);

        PurePursuit2DPathTracker tracker;
        tracker.update(path);
        tracker.set_goal(*node2);
        tracker.set_n_sampling_points(4);
        tracker.set_lookahead_dist(0.2);

        /// let's track
        tracker.execute(state);

    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
