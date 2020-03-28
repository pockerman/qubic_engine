#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/control/pure_pursuit_path_tracker.h"
#include "kernel/discretization/line_mesh.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/discretization/edge_element.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/discretization/node.h"
#include "kernel/vehicles/difd_drive_vehicle.h"


#include <array>
#include <iostream>

namespace example
{
using cengine::uint_t;
using cengine::real_t;
using cengine::control::PurePursuit2DPathTracker;
using kernel::numerics::LineMesh;
using kernel::GeomPoint;
using kernel::dynamics::SysState;

const real_t DT = 0.5;

/// An agent to simulate
class Agent
{
public:

    /// Constructor
    Agent(const LineMesh<2>& path, const GeomPoint<2>& goal, real_t goal_r);

    /// execute the agent i.e. go to goal
    void execute();

    /// returns trus  if the goal has been reached
    bool goal_reched()const;

private:

    /// the path the agent must follow
    const LineMesh<2>* path_ptr_;

    /// mobile platform of the agent
    kernel::DiffDriveVehicle platform_;

    /// the PP tracker
    PurePursuit2DPathTracker tracker_;

    /// the goal of the robot
    kernel::GeomPoint<2> goal_;

    /// the radius around the goal
    real_t g_radius_;
};

Agent::Agent(const LineMesh<2>& path, const GeomPoint<2>& goal, real_t goal_r)
    :
    path_ptr_(&path),
    platform_(0.2),
    tracker_(),
    goal_(goal),
    g_radius_(goal_r)
{}

bool
Agent::goal_reched()const{

    GeomPoint<2> pos;
    platform_.get_position(pos);

    if(pos.distance(goal_)<g_radius_){
        return true;
    }

    return false;
}

void Agent::execute(){


    /// update the path the tracker is using
    tracker_.update(*path_ptr_);
    tracker_.set_n_sampling_points(4);
    tracker_.set_lookahead_dist(0.2);

    real_t time = 0.0;

    while(!goal_reched()){

        /// let's see the state of the agent
        std::cout<<"At time: "<<time<<std::endl;
        std::cout<<platform_.get_state()<<std::endl;

        platform_.integrate(0.0, 0.0);

        /// correct for the path
        tracker_.execute(platform_.get_state());

        time += DT;
    }
}

}


int main(){

    using namespace example;

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

        Agent agent(path, *node2, 0.2);
        agent.execute();
    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
