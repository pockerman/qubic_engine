#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/control/carrot_chasing.h"
#include "cubic_engine/grids/waypoint_path.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/vehicles/differential_drive_vehicle.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/base/unit_converter.h"

#include <iostream>

namespace example{
using cengine::uint_t;
using cengine::real_t;
using cengine::control::CarrotChasingPathTrackController;
using cengine::grids::WaypointPath;
using cengine::grids::WayPoint;
using cengine::grids::LineSegment;
using cengine::grids::LineSegmentData;
using kernel::GeomPoint;
using kernel::dynamics::SysState;
using kernel::vehicles::DiffDriveConfig;
using kernel::Null;

const real_t DT = 0.1;
const real_t GOAL_RADIUS = 0.2; // m

/// An agent to simulate
class Agent
{
public:

    /// Constructor
    Agent(CarrotChasingPathTrackController<Null, LineSegmentData>& path_ctrl_,
          const GeomPoint<2>& goal, real_t goal_r,
          const  DiffDriveConfig& properties);

    /// execute the agent that is go to goal
    void execute();

    /// returns trus  if the goal has been reached
    bool goal_reached()const;

    /// set the initial position
    void set_init_pos(real_t x, real_t y);

    /// set the initial velocity
    void set_init_velocities(real_t v, real_t w);

    /// set the initial heading
    void set_init_orientation(real_t theta);

private:

    /// the path track controller
    CarrotChasingPathTrackController<Null, LineSegmentData>* controller_ptr_;

    /// mobile platform of the agent
    kernel::vehicles::DiffDriveVehicle platform_;

    /// the goal of the robot
    kernel::GeomPoint<2> goal_;

    /// the radius around the goal
    real_t g_radius_;

    /// save the results
    kernel::CSVWriter writer_;
};

Agent::Agent(CarrotChasingPathTrackController<Null, LineSegmentData>& path_ctrl,
             const GeomPoint<2>& goal,
             real_t goal_r, const  DiffDriveConfig& properties)
    :
    controller_ptr_(&path_ctrl),
    platform_(properties),
    goal_(goal),
    g_radius_(goal_r),
    writer_("agent_state.csv", ',', true)
{
    platform_.set_time_step(DT);
}

bool
Agent::goal_reached()const{

    GeomPoint<2> pos;
    platform_.get_position(pos);

    if(pos.distance(goal_)<=g_radius_){
        return true;
    }

    return false;
}

void
Agent::set_init_pos(real_t x, real_t y){
    platform_.set_x_position(x);
    platform_.set_y_position(y);
}

void
Agent::set_init_velocities(real_t v, real_t w){
    platform_.set_velocities(v, w);
}

void
Agent::set_init_orientation(real_t theta){
    platform_.set_orientation(theta);
}

void Agent::execute(){

    real_t time = 0.0;

    writer_.write_column_names({"X", "Y", "Theta", "XL", "YL"});

    auto v = 0.5;
    auto w = 0.0;

    std::vector<real_t> row(5, 0.0);
    row[0] = platform_.get_x_position();
    row[1] = platform_.get_y_position();
    row[2] = platform_.get_orientation();
    row[3] = platform_.get_x_position();
    row[4] = platform_.get_y_position();
    writer_.write_row(row);

    uint_t counter = 0;
    while(!goal_reached() && counter < 4250){

        std::cout<<"At time: "<<time<<std::endl;
        std::cout<<"\n";

        auto x = platform_.get_x_position();
        auto y = platform_.get_y_position();
        auto theta = platform_.get_orientation();
        std::cout<<"Position: "<<x<<" , "<<y<<" theta (degs) "<<kernel::UnitConverter::rad_to_degrees(theta)<<std::endl;

        std::cout<<"Current V: "<<v<<std::endl;
        std::cout<<"Current W: "<<w<<std::endl;
        
        auto [control_result, lookahed_point, closest] = controller_ptr_->execute(platform_.get_state());
        w = control_result;

        platform_.integrate(v, w);

        row[0] = platform_.get_x_position();
        row[1] = platform_.get_y_position();
        row[2] = platform_.get_orientation();
        row[3] = lookahed_point[0];
		row[4] = lookahed_point[1];
        writer_.write_row(row);

        time += DT;
        counter++;
    }
}

}


int main(){

    using namespace example;

    try{
           bool case_2 = true;

           if(!case_2)
           {
            WaypointPath<2, Null, LineSegmentData> path;
            path.reserve_nodes(6);

            auto start = path.add_node(GeomPoint<2>({0.0, 0.0}), Null());
            path.add_node(GeomPoint<2>({10.0, 0.0}), Null());
            path.add_node(GeomPoint<2>({20.0, 0.0}), Null());
            path.add_node(GeomPoint<2>({50.0, 30.0}), Null());
            path.add_node(GeomPoint<2>({60.0, 30.0}), Null());
            auto goal = path.add_node(GeomPoint<2>({90.0, 0.0}), Null());

            LineSegmentData segmentdata = {1.0, 0.0, 0.0, 0.0};

            path.reserve_elements(5);
            path.add_element(0, 1, segmentdata);
            path.add_element(1, 2, segmentdata);

            segmentdata.theta = kernel::UnitConverter::degrees_to_rad(45.0);

            path.add_element(2, 3, segmentdata);
            segmentdata.theta = 0.0;
            path.add_element(3, 4, segmentdata);

            segmentdata.theta = kernel::UnitConverter::degrees_to_rad(-45.0);
            path.add_element(4, 5, segmentdata);

            CarrotChasingPathTrackController<Null, LineSegmentData> controller;
            controller.set_lookahead_dist(1.0);
            controller.set_n_sampling_points(10);
            controller.set_gain(1.0);
            controller.set_wpoint_radius(1.5);

            controller.update(path);

            DiffDriveConfig properties;
            properties.wheel_radius = 1.; //m
            properties.width = 0.5; //m
            properties.vmax = 2.0; //m/sec

            /// the agent we simulate
            Agent agent(controller, *goal, GOAL_RADIUS, properties);

            /// initialze the state of the agent at (0,0.5)
            agent.set_init_pos(0.0, 5.0);

            /// set the linear and angular velocities
            agent.set_init_velocities(1.0, 0.0);

            /// set the initial heading
            agent.set_init_orientation(0.0);

            agent.execute();
          }
          else
          {

            WaypointPath<2, Null, LineSegmentData> path;
            path.reserve_nodes(19);

            auto start = path.add_node(GeomPoint<2>({0.5, 0.5}), Null());
            path.add_node(GeomPoint<2>({1.5, 0.5}), Null());
            path.add_node(GeomPoint<2>({2.5, 0.5}), Null());
            path.add_node(GeomPoint<2>({3.5, 0.5}), Null());
            path.add_node(GeomPoint<2>({3.5, 1.5}), Null());
            path.add_node(GeomPoint<2>({3.5, 2.5}), Null());
            path.add_node(GeomPoint<2>({3.5, 3.5}), Null());
            path.add_node(GeomPoint<2>({4.5, 3.5}), Null());
            path.add_node(GeomPoint<2>({4.5, 4.5}), Null());
            path.add_node(GeomPoint<2>({5.5, 4.5}), Null());
            path.add_node(GeomPoint<2>({5.5, 5.5}), Null());
            path.add_node(GeomPoint<2>({6.5, 5.5}), Null());
            path.add_node(GeomPoint<2>({7.5, 5.5}), Null());
            path.add_node(GeomPoint<2>({7.5, 6.5}), Null());
            path.add_node(GeomPoint<2>({7.5, 7.5}), Null());
            path.add_node(GeomPoint<2>({7.5, 8.5}), Null());
            path.add_node(GeomPoint<2>({8.5, 8.5}), Null());
            path.add_node(GeomPoint<2>({9.5, 8.5}), Null());
            auto goal = path.add_node(GeomPoint<2>({9.5, 9.5}), Null());
            
            LineSegmentData segmentdata = {1.0, 0.0, 0.0, 0.0};

            path.reserve_elements(5);
            path.add_element(0, 1, segmentdata);
            path.add_element(1, 2, segmentdata);
            path.add_element(2, 3, segmentdata);
            path.add_element(3, 4, segmentdata);
            path.add_element(4, 5, segmentdata);
            path.add_element(5, 6, segmentdata);
            path.add_element(6, 7, segmentdata);
            path.add_element(7, 8, segmentdata);
            path.add_element(8, 9, segmentdata);
            path.add_element(9, 10, segmentdata);
            path.add_element(10, 11, segmentdata);
            path.add_element(11, 12, segmentdata);
			path.add_element(12, 13, segmentdata);
			path.add_element(13, 14, segmentdata);
			path.add_element(14, 15, segmentdata);
			path.add_element(15, 16, segmentdata);
			path.add_element(16, 17, segmentdata);
			path.add_element(17, 18, segmentdata);
			
            CarrotChasingPathTrackController<Null, LineSegmentData> controller;
            controller.set_lookahead_dist(0.4);
            controller.set_n_sampling_points(10);
            controller.set_gain(1.0);
            controller.set_wpoint_radius(1.5);

            controller.update(path);

            DiffDriveConfig properties;
            properties.wheel_radius = 1.; //m
            properties.width = 0.5; //m
            properties.vmax = 2.0; //m/sec

            /// the agent we simulate
            Agent agent(controller, *goal, GOAL_RADIUS, properties);

            /// initialze the state of the agent at (0,0.5)
            agent.set_init_pos(0.5, 0.5);

            /// set the linear and angular velocities
            agent.set_init_velocities(.5, 0.0);

            /// set the initial heading
            agent.set_init_orientation(0.0);

            agent.execute();

          }

    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
