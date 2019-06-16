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

namespace exe2{

using cengine::DynVec;
using cengine::DynMat;
using cengine::real_t;
using cengine::uint_t;

const uint_t N_STEPS = 1000;
const  real_t DT = 0.1;


// The class that describes the dynamics of the robot
// The following model is useed
// $$ \dot{x} = vcos(\phi)$$
// $$ \dot{y} = vsin((\phi)$$
// $$ \dot{\phi} = \omega$$

class Dynamics
{

public:

    template<typename... Args>
    void execute(const Args&... args);

private:



};

struct MotionModel: public cengine::EKF_F_func
{
    virtual void operator()(DynVec<real_t>& x, const DynVec<real_t>& x_prev, const DynMat<real_t>& A,
                            const DynMat<real_t>& B, const DynVec<real_t>& u  )const override final;
};

struct ObservationModel: public cengine::EKF_H_func
{
   virtual DynVec<real_t> operator()(const DynVec<real_t>& x)const override final;
};

// The robot to simulate
class Robot
{
public:

    void simulate(DynVec<real_t>& u, const DynVec<real_t>& y);

    // initialize the robot
    void initialize();

private:

    // the state of the robot:
    // x, y are a 2D x-y position, $\phi$ is orientation, and v is velocity
    DynVec<real_t> state_;

    // matrix that describe the state of the robot
    DynMat<real_t> A_;

    // matrix that describes the input
    DynMat<real_t> B_;

    // The covariance matrix
    DynMat<real_t> P_;

    // The covariance matrix
    DynMat<real_t> Q_;

    DynMat<real_t> L_;

    // The object that handles the dynamics of the robot
    Dynamics dynamics_;

    // The state estimator of the robot
    cengine::ExtendedKalmanFilter state_estimator_;

    // update the state matrix A_
    void update_A_mat();

    // update the B_ matrix
    void update_B_mat();

    // update the P_ matrix
    void update_P_mat();

    void update_Q_mat();
    void update_L_mat();

    // The motion model the rovot is using
    MotionModel f_func_;

    // The observation model the robot is using
    ObservationModel h_func_;


};

}//exe2



#endif	/* EXE_H */

