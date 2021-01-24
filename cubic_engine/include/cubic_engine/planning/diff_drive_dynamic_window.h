#ifndef DIFF_DRIVE_DYNAMIC_WINDOW_H
#define DIFF_DRIVE_DYNAMIC_WINDOW_H

#include "cubic_engine/planning/dynamic_window.h"
namespace cengine
{
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
      real_t max_dyawrate;
      real_t dt;
      real_t min_cost;
      real_t v_reso;
      real_t yawrate_reso;
};

struct DiffDriveWindowProperties
{
    real_t v_max;
    real_t v_min;
    real_t w_max;
    real_t w_min;
};

///
/// \brief DiffDriveDW class. Dynamic window approach for
/// differential drive systems. This class is an implementation
/// from: https://github.com/onlytailei/CppRobotics/blob/master/src/dynamic_window_approach.cpp
///
template<typename StateTp, typename GoalTp>
class DiffDriveDW: public DynamicWindowBase<StateTp, DiffDriveDWConfig, DiffDriveWindowProperties>
{
public:

    ///
    /// \brief state_t. The type of the state used by the window
    ///
    typedef typename DynamicWindowBase<StateTp, DiffDriveDWConfig, DiffDriveWindowProperties>::state_t state_t;


    ///
    /// \brief config_t. Congiguration type used by the window
    ///
    typedef typename DynamicWindowBase<StateTp, DiffDriveDWConfig, DiffDriveWindowProperties>::config_t config_t;

    ///
    /// \brief window_properties_t
    ///
    typedef typename DynamicWindowBase<StateTp, DiffDriveDWConfig, DiffDriveWindowProperties>::window_properties_t window_properties_t;

    ///
    /// \brief goal_t. The type of the goal
    ///
    typedef GoalTp goal_t;

    ///
    /// \brief control_t The type of the control input calculated by the
    /// the application of the window
    ///
    typedef std::vector<real_t> control_t;

    ///
    /// \brief DiffDriveDW. Constructor
    ///
    DiffDriveDW(state_t& state, const config_t& config, const goal_t& goal, const control_t& control);


    ///
    /// \brief calculate_window. Calculate the window
    ///
    virtual window_properties_t& calculate_window()final override;

    ///
    /// \brief Given the obstacle type calculate the appropriate control
    ///
    template<typename ObstacleTp>
    void dwa_control(const ObstacleTp& obstacle);


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
    /// \brief calculate_control_input_ Calculate the control inputs
    ///
    template<typename ObstacleTp>
    void calculate_control_input_(const ObstacleTp& obstacle);

};

template<typename StateTp, typename GoalTp>
DiffDriveDW<StateTp, GoalTp>::DiffDriveDW(state_t& state, const config_t& config, const goal_t& goal, const control_t& control)
    :
    DynamicWindowBase<StateTp, DiffDriveDWConfig, DiffDriveWindowProperties>(state, config),
    goal_(goal),
    control_(control)
{}

template<typename StateTp, typename GoalTp>
typename DiffDriveDW<StateTp, GoalTp>::window_properties_t&
DiffDriveDW<StateTp, GoalTp>::calculate_window(){

    this->w_properties_.v_min = std::max((*(this->state_)["v"] - this->config_.max_accel * this->config_.dt), this->config_.min_speed);
    this->w_properties_.v_max = std::min((*(this->state_)["v"] + this->config_.max_accel * this->config_.dt), this->config_.max_speed);
    this->w_properties_.w_min = std::max((*(this->state_)["w"] - this->config_.max_dyawrate * this->config_.dt), -this->config_.max_yawrate);
    this->w_properties_.w_max = std::min((*(this->state_)["w"] + this->config_.max_dyawrate * this->config_.dt), this->config_.max_yawrate);

    return this->w_properties_;
}

template<typename StateTp, typename GoalTp>
template<typename ObstacleTp>
void
DiffDriveDW<StateTp, GoalTp>::calculate_control_input_(const ObstacleTp& obstacle){

    float min_cost = 10000.0;
    auto min_u = control_;
    min_u[0] = 0.0;

    auto v_min = this->w_properties_.v_min;
    auto v_max = this->w_properties_.v_max;
    auto w_min = this->w_properties_.w_min;
    auto w_max = this->w_properties_.w_max;

        //Traj best_traj;

        // evalucate all trajectory with sampled input in dynamic window
        for (auto v=v_min; v <= v_max; v += this->config_.v_reso){
          for (auto y=w_min; y <= w_max; y += this->config_.yawrate_reso){

            /*Traj traj = calc_trajectory(x, v, y, config);

            float to_goal_cost = calc_to_goal_cost(traj, goal, config);
            float speed_cost = config.speed_cost_gain * (config.max_speed - traj.back()[3]);
            float ob_cost = calc_obstacle_cost(traj, ob, config);
            float final_cost = to_goal_cost + speed_cost + ob_cost;

            if (min_cost >= final_cost){
              min_cost = final_cost;
              min_u = Control{{v, y}};
              best_traj = traj;*/
            }
          }

        control_ = min_u;
}

template<typename StateTp, typename GoalTp>
template<typename ObstacleTp>
void
DiffDriveDW<StateTp, GoalTp>::dwa_control(const ObstacleTp& obstacle){

    // calculate the window
    auto& window = calculate_window();
}

}
}

#endif // DIFF_DRIVE_DYNAMIC_WINDOW_H
