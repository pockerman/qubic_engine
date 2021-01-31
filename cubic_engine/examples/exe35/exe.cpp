#include "cubic_engine/base/config.h"

#ifdef USE_OPEN_CV

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/planning/diff_drive_dynamic_window.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/vehicles/difd_drive_vehicle.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/maths/constants.h"

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include <opencv2/imgproc.hpp>


#include<iostream>
#include<vector>
#include<array>
#include<cmath>


namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::DynVec;
using kernel::GeomPoint;
using kernel::DiffDriveVehicle;
using kernel::DiffDriveConfig;
using kernel::dynamics::SysState;
using cengine::planning::DiffDriveWindowProperties;
using cengine::planning::DiffDriveDW;
using cengine::planning::DiffDriveDWConfig;

const real_t DT = 0.1;
const uint_t N_MAX_ITRS = 1000;
const real_t ROBOT_RADIUS = 1.0;

typedef DiffDriveDW<SysState<5>, GeomPoint<2>>::trajectory_t trajectory_t;

cv::Point2i
cv_offset(float x, float y, int image_width=2000, int image_height=2000){
  cv::Point2i output;
  output.x = static_cast<int>(x * 100) + image_width/2;
  output.y = image_height - static_cast<int>(y * 100) - image_height/3;
  return output;
}

void update_state_from_vehicle(SysState<5>& state, const DiffDriveVehicle& vehicle){

    state[0] = vehicle.get_x_position();
    state[1] = vehicle.get_y_position();
    state[2] = vehicle.get_orientation();
    state[3] = vehicle.get_velocity();
    state[4] = vehicle.get_w_velocity();

}

void update_trajectory(trajectory_t& trajectory, const SysState<5>& state){

    auto state_values = state.get_values();
    trajectory.push_back(state_values);
}

}

int main() {
   
    using namespace cv;
    using namespace example;
    using Obstacle = std::vector<std::array<float, 2>>;

    // a list of obstacles
    Obstacle ob({
        {{-1, -1}},
        {{0, 2}},
        {{4.0, 2.0}},
        {{5.0, 4.0}},
        {{5.0, 5.0}},
        {{5.0, 6.0}},
        {{5.0, 9.0}},
        {{8.0, 9.0}},
        {{7.0, 9.0}},
        {{12.0, 12.0}}
      });

    DiffDriveConfig config;
    config.Vmax = 1.0;

    DiffDriveVehicle vehicle(config);

    vehicle.set_time_step(DT);
    vehicle.set_x_position(0.0);
    vehicle.set_y_position(0.0);
    vehicle.set_orientation(kernel::MathConsts::PI/8.0);

    SysState<5> x({"x", "y", "theta", "v", "w"}, 0.0);
    update_state_from_vehicle(x, vehicle);

    // the goal
    GeomPoint<2> goal={10.0,10.0};

    typedef DiffDriveDW<SysState<5>, GeomPoint<2>>::control_t control_t;

    control_t control{0.0, 0.0};

    DiffDriveDWConfig dw_config;
    dw_config.robot_radius = ROBOT_RADIUS;
    dw_config.skip_n = 2;
    dw_config.dt = DT;
    dw_config.predict_time = 3.0;
    dw_config.max_speed = 1.0;
    dw_config.min_speed = -0.5;
    dw_config.max_yaw_rate = 40.0 * kernel::MathConsts::PI / 180.0;
    dw_config.max_accel = 0.2;
    dw_config.max_dyawrate = 40.0 * kernel::MathConsts::PI / 180.0;
    dw_config.v_reso = 0.01;
    dw_config.yawrate_reso = 0.1 * kernel::MathConsts::PI / 180.0;

    DiffDriveWindowProperties wproperties;

    // the dynamic window
    DiffDriveDW<SysState<5>, GeomPoint<2>> dw(x, dw_config, goal, control, wproperties);

    trajectory_t traj;
    update_trajectory(traj, x);

    try{

          bool terminal = false;

          // OpenCV window
          cv::namedWindow("DWA", cv::WINDOW_NORMAL);
          int count = 0;

          for(auto i=0; i<N_MAX_ITRS && !terminal; i++){

            // calculate best trajectory
            auto trajectory = dw.dwa_control(ob);

            // integrate the vehicle based on the
            // updated controls
            vehicle.integrate(dw.get_control()[0], dw.get_control()[1]);

            update_state_from_vehicle(x, vehicle);

            update_trajectory(traj, x);

            // visualization
            cv::Mat bg(3500,3500, CV_8UC3, cv::Scalar(255,255,255));

            cv::circle(bg, cv_offset(goal[0], goal[1], bg.cols, bg.rows), 30, cv::Scalar(255,0,0), 5);

            for(auto j=0; j<ob.size(); j++){
                 cv::circle(bg, cv_offset(ob[j][0], ob[j][1], bg.cols, bg.rows), 20, cv::Scalar(0,0,0), -1);
            }

            for(auto j=0; j<trajectory.size(); j++){
                  cv::circle(bg, cv_offset(trajectory[j][0], trajectory[j][1],
                                            bg.cols, bg.rows), 7, cv::Scalar(0,255,0), -1);
            }

            cv::circle(bg, cv_offset(x[0], x[1], bg.cols, bg.rows), 30, cv::Scalar(0,0,255), 5);

            cv::arrowedLine(
                  bg,
                  cv_offset(x[0], x[1], bg.cols, bg.rows),
                  cv_offset(x[0] + std::cos(x[2]), x[1] + std::sin(x[2]), bg.cols, bg.rows),
                  cv::Scalar(255,0,255),
                  7);

            // check if goal is reached
            if (std::sqrt(std::pow((x[0] - goal[0]), 2) + std::pow((x[1] - goal[1]), 2)) <= ROBOT_RADIUS){
                  terminal = true;
                  for(auto j=0; j<traj.size(); j++){
                         cv::circle(bg, cv_offset(traj[j][0], traj[j][1], bg.cols, bg.rows),
                                     7, cv::Scalar(0,0,255), -1);
                       }
            }

            cv::imshow("DWA", bg);
            cv::waitKey(5);

            // std::string int_count = std::to_string(count);
            // cv::imwrite("./pngs/"+std::string(5-int_count.length(), '0').append(int_count)+".png", bg);

            count++;
          }
    }
    catch(cv::Exception& e){
        std::cerr<<"OpenCV error: "
                 <<e.what()<<std::endl;
    }
    catch(std::runtime_error& e){
        std::cerr<<"Runtime error: "
                 <<e.what()<<std::endl;
    }
    catch(std::invalid_argument& e){
        std::cerr<<"Invalid argument error: "
                 <<e.what()<<std::endl;
    }
    catch(std::logic_error& e){
        std::cerr<<"Logic error: "
                 <<e.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running example."<<std::endl;
    }
   
    return 0;
}
#else
#include <iostream>
#include <stdexcept>
int main() {

   std::cerr<<"This example requires OpenCV. Reconfigure with OpenCV support"<<std::endl;
   throw std::runtime_error("No OpenCV support");
   return 0;
}
#endif

