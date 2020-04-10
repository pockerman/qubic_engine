#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/control/pure_pursuit_path_tracker.h"
#include "cubic_engine/control/bang_bang_path_track_controler.h"
#include "cubic_engine/grids/waypoint_path.h"
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
using cengine::control::BangBangPathTrackController;
using cengine::grids::WaypointPath;
using cengine::grids::WayPoint;
using cengine::grids::LineSegment;
using cengine::grids::LineSegmentData;
using kernel::GeomPoint;
using kernel::dynamics::SysState;
using kernel::DiffDriveProperties;
using kernel::Null;

const real_t DT = 0.5;

/// An agent to simulate
class Agent
{
public:

    /// Constructor
    Agent(const WaypointPath<2, Null, LineSegmentData>& path,
          const GeomPoint<2>& goal,
          real_t goal_r, const  DiffDriveProperties& properties);

    /// execute the agent that is go to goal
    void execute();

    /// returns trus  if the goal has been reached
    bool goal_reached()const;

private:

    /// the path the agent must follow
    const WaypointPath<2, Null, LineSegmentData>* path_ptr_;

    /// mobile platform of the agent
    kernel::DiffDriveVehicle platform_;

    /// the PP tracker
    PurePursuit2DPathTracker tracker_;

    /// the goal of the robot
    kernel::GeomPoint<2> goal_;

    /// the radius around the goal
    real_t g_radius_;
};

Agent::Agent(const WaypointPath<2, Null, LineSegmentData>& path,
             const GeomPoint<2>& goal,
             real_t goal_r, const  DiffDriveProperties& properties)
    :
    path_ptr_(&path),
    platform_(properties),
    tracker_(),
    goal_(goal),
    g_radius_(goal_r)
{}

bool
Agent::goal_reached()const{

    GeomPoint<2> pos;
    platform_.get_position(pos);

    if(pos.distance(goal_)<g_radius_){
        return true;
    }

    return false;
}

void Agent::execute(){


    /// update the path the tracker is using
    /// and the various properties
    tracker_.update(*path_ptr_);
    tracker_.set_n_sampling_points(4);
    tracker_.set_lookahead_dist(0.2);

    real_t time = 0.0;

    while(!goal_reached()){

        /// let's see the state of the agent
        std::cout<<"At time: "<<time<<std::endl;
        std::cout<<platform_.get_state()<<std::endl;

        /// correct for the path
        tracker_.execute(platform_.get_state());

        /// derive the ref vr and vl required to
        /// follow the path

        /// apply control to the derived vr_ref and
        /// vl_ref


        /// ...and feed the result back to the system

        platform_.integrate(0.0, 0.0);





        time += DT;
    }
}

}


int main(){

    using namespace example;

    const real_t GOAL_RADIUS = 0.2;
    try{

        /// the path to follow
        WaypointPath<2, Null, LineSegmentData> path;
        path.reserve_n_waypoints(3);

        path.add_way_point(GeomPoint<2>({0.0, 0.0}), Null());
        path.add_way_point(GeomPoint<2>({1.0, 1.0}), Null());
        path.add_way_point(GeomPoint<2>({2.0, 0.0}), Null());

        path.reserve_n_segments(2);
        path.add_segment(0, 1, LineSegmentData());
        path.add_segment(1, 2, LineSegmentData());

        DiffDriveProperties properties;

        Agent agent(path, *node2, GOAL_RADIUS, properties);
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
