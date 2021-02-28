
#include "cubic_engine/base/config.h"

#ifdef USE_PLANNING

#include "cubic_engine/planning/diff_drive_dynamic_window.h"


namespace cengine{
namespace planning{

DiffDriveDW::DiffDriveDW(const state_t& state, const config_t& config, const goal_t& goal,
                                  const control_t& control)
    :
    goal_(goal),
    control_(control),
    state_(state),
    config_(config)
{}

DiffDriveDW::window_properties_t&
DiffDriveDW::calculate_window(){

    // update the dynamic window
    // properties.
    w_properties_.v_min = std::max(state_["v"] - config_.max_accel * config_.dt, config_.min_speed);
    w_properties_.v_max = std::min(state_["v"] + config_.max_accel * config_.dt, config_.max_speed);
    w_properties_.w_min = std::max(state_["w"] - config_.max_delta_yaw_rate * config_.dt, -config_.max_yaw_rate);
    w_properties_.w_max = std::min(state_["w"] + config_.max_delta_yaw_rate * config_.dt, config_.max_yaw_rate);

    return this->w_properties_;
}

DiffDriveDW::trajectory_t
DiffDriveDW::predict_trajectory_(real_t v, real_t w){

    trajectory_t traj;
    traj.push_back(state_.get_values());

    typedef kernel::dynamics::DiffDriveDynamics::input_t input_t;
    input_t model_input;
    model_input.insert({"v", v});
    model_input.insert({"w", w});
    model_input.insert({"dt", config_.dt});

#ifdef USE_WARNINGS_FOR_MISSING_IMPLEMENTATION
    std::cout<<kernel::KernelConsts::warning_str()<<"Errors have not been accounted for in the implementation"<<std::endl;
#endif

    std::array<real_t, 2> errors;
    errors[0] = errors[1] = 0.0;
    model_input.insert({"errors", errors});

    auto time = 0.0;
    kernel::dynamics::SysState<3> init_state({"x", "y", "theta"}, 0.0);
    kernel::dynamics::SysState<3>::extract(state_, init_state);
    kernel::dynamics::SysState<5> tmp = state_;
    tmp["v"] = v;
    tmp["w"] = w;

    while (time <= config_.predict_time){

        auto result = kernel::dynamics::DiffDriveDynamics::integrate(init_state, model_input, config_.dynamics_version);
        tmp["x"] = result["x"];
        tmp["y"] = result["y"];
        tmp["theta"] = result["theta"];
        init_state["x"] = result["x"];
        init_state["y"] = result["y"];
        init_state["theta"] = result["theta"];

        traj.push_back(tmp.get_values());
        time += config_.dt;
    }

    return traj;
}

real_t
DiffDriveDW::calc_to_goal_cost_(const trajectory_t& trajectory){

    auto dx = goal_[0] - trajectory.back()[0];
    auto dy = goal_[1] - trajectory.back()[1];
    auto error_angle = std::atan2(dy, dx);
    auto cost_angle = error_angle - trajectory.back()[2];

    return std::abs(std::atan2(std::sin(cost_angle), std::cos(cost_angle)));
}

}
}

#endif
