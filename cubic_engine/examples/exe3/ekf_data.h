#ifndef EKF_DATA_H
#define EKF_DATA_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/estimation/extended_kalman_filter.h"

namespace exe
{

using cengine::DynVec;
using cengine::DynMat;
using cengine::real_t;
using cengine::uint_t;

uint_t Rate = 10;

real_t DT = 1.0/Rate;

constexpr auto STATE_SIZE = 4;

// the state of the robot:
// x, y are a 2D x-y position, $\phi$ is orientation, and v is velocity
DynVec<real_t> state(STATE_SIZE, 0.0);

// matrix that describe the state of the robot
DynMat<real_t> A;
DynMat<real_t> F;

// matrix that describes the input
DynMat<real_t> B;

// The covariance matrix
DynMat<real_t> P;

// The covariance matrix
DynMat<real_t> Q;

DynMat<real_t> L;
DynMat<real_t> H;
DynMat<real_t> Hjac;
DynMat<real_t> M;
DynMat<real_t> R;

// The externally applied input
DynVec<real_t> u(2, 0.0);
DynVec<real_t> y(2, 0.0);

// The state estimator of the robot
/*cengine::ExtendedKalmanFilter state_estimator;

// The motion model the rovot is using
typedef cengine::EKF_F_func MotionModel;
MotionModel motion_model;

// The observation model the robot is using
typedef cengine::EKF_H_func ObservationModel;
ObservationModel h_func;

*/

// Helper functions
void update_A_mat();
void update_F_mat();
void update_B_mat();
void update_P_mat();
void update_Q_mat();
void update_L_mat();
void update_H_mat();
void update_Hjac_mat();
void update_M_mat();
void update_R_mat();

void initialize_ekf();

}

#include "ekf_data.cpp"

#endif // EKF_DATA_H


