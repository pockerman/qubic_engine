/* 
 * File:   exe.h
 * Author: david
 *
 * Created on October 25, 2015, 10:26 AM
 */

#ifndef EXE_H
#define	EXE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/estimation/extended_kalman_filter.h"


namespace kernel
{
class CSVWriter;
}

namespace exe2{

using cengine::DynVec;
using cengine::DynMat;
using cengine::real_t;
using cengine::uint_t;

const uint_t N_STEPS = 1000;
const  real_t DT = 0.1;

// The robot to simulate
class Robot
{
public:



    // constructor
    Robot();

    void simulate(DynVec<real_t>& u, const DynVec<real_t>& y);

    // initialize the robot
    void initialize();

    // set the input
    void set_input(const DynVec<real_t>& v){u_ = &v;}

    // save the current state
    void save_state(kernel::CSVWriter& writer)const;

    // Apply the motion model and return a state vector
    void apply_motion_model(DynVec<real_t>& x, const DynVec<real_t>& u)const;



private:

    // the state of the robot:
    // x, y are a 2D x-y position, $\phi$ is orientation, and v is velocity
    DynVec<real_t> state_;

    // matrix that describe the state of the robot
    DynMat<real_t> A_;
    DynMat<real_t> F_;

    // matrix that describes the input
    DynMat<real_t> B_;

    // The covariance matrix
    DynMat<real_t> P_;

    // The covariance matrix
    DynMat<real_t> Q_;

    DynMat<real_t> L_;
    DynMat<real_t> H_;
    DynMat<real_t> Hjac_;
    DynMat<real_t> M_;
    DynMat<real_t> R_;

    // The externally applied input
    const DynVec<real_t>* u_;

    // The state estimator of the robot
    cengine::ExtendedKalmanFilter state_estimator_;

    // The motion model the rovot is using
    typedef cengine::EKF_F_func MotionModel;
    MotionModel motion_model_;

    // The observation model the robot is using
    typedef cengine::EKF_H_func ObservationModel;
    ObservationModel h_func_;

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



};

}//exe2



#endif	/* EXE_H */

