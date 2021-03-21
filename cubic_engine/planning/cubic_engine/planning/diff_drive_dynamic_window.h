#ifndef DIFF_DRIVE_DYNAMIC_WINDOW_H
#define DIFF_DRIVE_DYNAMIC_WINDOW_H
#include "cubic_engine/base/config.h"

#ifdef USE_PLANNING

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/dynamics/diff_drive_dynamics.h"
#include "kernel/geometry/bounding_box_type.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/geometry/geom_point.h"

#include <vector>
#include <array>
#include <limits>
#include <cmath>

#ifdef USE_WARNINGS_FOR_MISSING_IMPLEMENTATION
#include <iostream>
#include "kernel/base/kernel_consts.h"
#endif

namespace cengine{
namespace planning {

///
/// \brief The DiffDriveDWConfig struct. Helper
/// struct to hold configuration properties
/// for the DiffDriveDW class
///
struct DiffDriveDWConfig
{

      real_t max_speed;
      real_t min_speed;
      real_t max_yaw_rate;
      real_t max_accel;
      real_t max_delta_yaw_rate;
      real_t dt;
      real_t min_cost;
      real_t v_reso;
      real_t yawrate_reso;
      real_t robot_radius;
      uint_t skip_n;
      real_t predict_time;
      real_t speed_cost_gain;
      real_t to_goal_cost_gain;
      real_t obstacle_cost_gain;
      real_t robot_stuck_flag_cons;
      kernel::dynamics::DiffDriveDynamics::DynamicVersion dynamics_version;
      kernel::geom::BBType bbtype;
      bool show_warnigs{true};
};

///
/// \brief The DiffDriveWindowProperties struct
///
struct DiffDriveWindowProperties
{
    real_t v_max{0.0};
    real_t v_min{0.0};
    real_t w_max{0.0};
    real_t w_min{0.0};
};

///
/// \brief DiffDriveDW class. Dynamic window approach for
/// differential drive systems. This class is an implementation
/// from: https://github.com/onlytailei/CppRobotics/blob/master/src/dynamic_window_approach.cpp
///
class DiffDriveDW
{
public:

    ///
    /// \brief state_t. The type of the state used by the window
    ///
    typedef kernel::dynamics::SysState<5> state_t;

    ///
    /// \brief goal_t. The type of the goal
    ///
    typedef kernel::GeomPoint<2> goal_t;

    ///
    /// \brief control_t The type of the control input calculated by the
    /// the application of the window
    ///
    typedef std::vector<real_t> control_t;

    ///
    /// \brief trajectory_t Helper type used to form trajectories
    ///
    typedef std::vector<std::array<real_t, 5>> trajectory_t;

    ///
    /// \brief config_t. Congiguration type used by the window
    ///
    typedef DiffDriveDWConfig config_t;

    ///
    /// \brief window_properties_t
    ///
    typedef DiffDriveWindowProperties window_properties_t;

    ///
    /// \brief DiffDriveDW. Constructor
    ///
    DiffDriveDW(const state_t& state, const config_t& config,
                const goal_t& goal, const control_t& control);

    ///
    /// \brief get_control. Access the control value
    ///
    const control_t& get_control()const{return control_;}

    ///
    /// \brief calculate_window. Calculate the window
    ///
    window_properties_t& calculate_window();

    ///
    /// \brief Given the obstacle type calculate the appropriate control
    ///
    template<typename ObstacleTp>
    trajectory_t dwa_control(const ObstacleTp& obstacle);

    ///
    /// \brief update_dynamics_state. Update the state of
    /// the object describing the dynamics
    ///
    void update_state(const state_t& state){state_ = state;}

protected:

    ///
    /// \brief goal_ The goal of the robot
    ///
    goal_t goal_;

    ///
    /// \brief control_. The initial control input
    ///
    control_t control_;

    ///
    /// \brief state_. The state descrbing the
    /// robot that the algorithm uses internally
    ///
    kernel::dynamics::SysState<5> state_;

    ///
    /// \brief config_. The congiration of thw window
    /// for Window calculation
    ///
    config_t config_;

    ///
    /// \brief w_properties_ The window properties. Updated
    /// every time calculate_window is called
    ///
    window_properties_t w_properties_;

    ///
    /// \brief calculate_control_input_ Calculate the control inputs
    ///
    template<typename ObstacleTp>
    trajectory_t calculate_control_input_(const ObstacleTp& obstacle);

    ///
    /// \brief calc_trajectory_
    ///
    trajectory_t predict_trajectory_(real_t v, real_t w);

    ///
    /// \brief calc_to_goal_cost_ Claculate the cost to the goal with the
    /// given trajectory
    ///
    real_t calc_to_goal_cost_(const trajectory_t& trajectory);

    ///
    /// \brief calc_obstacle_cost_ Calculate the cost to obstacle
    /// on the given trajectory. The cost is infinite if
    /// collision occurs and 0 if not.
    ///
    template<typename ObstacleTp>
    real_t calc_obstacle_cost_(const trajectory_t& trajectory, const ObstacleTp& obstacle);
};

template<typename ObstacleTp>
typename DiffDriveDW::trajectory_t
DiffDriveDW::dwa_control(const ObstacleTp& obstacle){

    // calculate the window
    auto& window = calculate_window();

    // calculate the control values based on the
    // updated window properties
    return calculate_control_input_(obstacle);
}

template<typename ObstacleTp>
real_t
DiffDriveDW::calc_obstacle_cost_(const trajectory_t& trajectory, const ObstacleTp& obstacle){

    // calc obstacle cost max (inf): collistion, 0:free
    auto minr = std::numeric_limits<real_t>::max();

    for (auto ii=0; ii<trajectory.size(); ii += this->config_.skip_n){
      for (auto i=0; i< obstacle.size(); i++){

        auto ox = obstacle[i][0];
        auto oy = obstacle[i][1];
        auto dx = trajectory[ii][0] - ox;
        auto dy = trajectory[ii][1] - oy;

        float r = std::sqrt(dx*dx + dy*dy);

        if (r <= this->config_.robot_radius){
            return std::numeric_limits<real_t>::max();
        }

        if (minr >= r){
            minr = r;
        }
      }
    }

    return 1.0 / minr;
}



template<typename ObstacleTp>
DiffDriveDW::trajectory_t
DiffDriveDW::calculate_control_input_(const ObstacleTp& obstacle){

    auto min_cost = std::numeric_limits<real_t>::max(); //this->config_.min_cost;
    auto min_u = control_;
    min_u[0] = 0.0;
    min_u[1] = 0.0;

    auto v_min = w_properties_.v_min;
    auto v_max = w_properties_.v_max;
    auto w_min = w_properties_.w_min;
    auto w_max = w_properties_.w_max;

    // the best trajectory
    trajectory_t best_traj;

    // evalucate all trajectory with sampled input in dynamic window
    for (auto v=v_min; v <= v_max; v += this->config_.v_reso){
       for (auto w=w_min; w <= w_max; w += this->config_.yawrate_reso){



            trajectory_t traj = predict_trajectory_(v, w);

            // what's the cost reaching goal on this
            // trajectory
            auto to_goal_cost = this->config_.to_goal_cost_gain*calc_to_goal_cost_(traj);
            auto speed_cost = this->config_.speed_cost_gain * (this->config_.max_speed - traj.back()[3]);
            auto ob_cost = this->config_.obstacle_cost_gain*calc_obstacle_cost_(traj, obstacle);

            // final cost
            auto final_cost = to_goal_cost + speed_cost + ob_cost;

            // we want minimum cost trajectory
            if (min_cost >= final_cost){
                  min_cost = final_cost;
                  min_u[0] = v;
                  min_u[1] = w; //control_t{{v, w}};
                  best_traj = traj;

                  if(std::fabs(min_u[0]) < config_.robot_stuck_flag_cons &&
                     std::fabs(state_["v"]) < config_.robot_stuck_flag_cons){
                       // to ensure the robot do not get stuck in
                       // best v=0 m/s (in front of an obstacle) and
                       // best omega=0 rad/s (heading to the goal with
                       // angle difference of 0)
                       min_u[1] = -config_.max_delta_yaw_rate;
                  }
            }
        }
    }

    // update the control and return the best
    // trajectory
    control_ = min_u;
    return best_traj;
}


}
}
#endif
#endif // DIFF_DRIVE_DYNAMIC_WINDOW_H
