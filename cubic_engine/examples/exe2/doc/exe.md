# Example 1

## Contents
* [Overview](#overview) 
* [Include files](#include_files)
* [Program structure](#prg_struct)
				* [The Robot class](#ch_robot)
				* [The RobotAI class](#rai_struct)
				* [The Robot class](#r_struct)
* [The main function](#m_func)
* [Results](#results)

## <a name="overview"></a> Overview

In this example we will develop an Extended Kalaman Filter or EKF for short. In you do not know what an EKF is or how it works have a look at here:
 

## <a name="include_files"></a> Include files
The include files, bring into our program scope functionality that our program needs in order to compile and run. 
 
```cpp

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/estimation/extended_kalman_filter.h"

#include "exe.h"
#include "parframe/base/angle_calculator.h"
#include "parframe/utilities/csv_file_writer.h"

#include <boost/math/distributions/normal.hpp> // normal distribution
#include <cmath>
#include <iostream>
#include <tuple>
```
## <a name="prg_struct"></a> Program structure

We will wrap the example in a separate namespace to keep things clean.

```cpp
namespace exe2
{
```

### <a name="ch_robot"></a> The Robot class

This class wraps information about the  robot. 


```cpp

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

```

The constructor does not do much. It simply initializes the private members to the desired values.

```cpp
Robot::Robot()
    :
      state_(),
      A_(),
      B_(),
      P_(),
      Q_(),
      L_(),
      H_(),
      M_(),
      R_(),
      u_(nullptr),
      state_estimator_(),
      motion_model_(),
      h_func_()
{}
```

Similarly, the functions that initialize the matrices used by the filter

```cpp
void
Robot::update_A_mat(){
     A_.resize(4, 4, 0.0);
     A_( 0,0 ) = 1.0;
     A_( 1,1 ) = 1.0;
     A_( 2,2 ) = 1.0;
}

void
Robot::update_F_mat(){

    F_.resize(4, 4);

    real_t yaw = state_[2];
    real_t v = (*u_)[0];

    F_(0, 0) = 1.0;
    F_(0, 1 ) = 0.0;
    F_(0, 2) = -DT * v * std::sin(yaw);
    F_(0, 3) =  DT * std::cos(yaw);

    F_(1, 0) = 0.0;
    F_(1, 1) = 1.0;
    F_(1, 2) = DT * v * std::cos(yaw);
    F_(1,3 ) = DT * std::sin(yaw);

    F_(2, 0 ) = 0.0;
    F_(2, 1 ) = 0.0;
    F_(2, 2 ) = 1.0;
    F_(2, 3 ) = 0.0;

    F_(3, 0 ) = 0.0;
    F_(3, 1 ) = 0.0;
    F_(3, 2 ) = 0.0;
    F_(3, 3 ) = 1.0;
}

void
Robot::update_P_mat(){
    P_ = std::move(cengine::IdentityMatrix<real_t>(4));
}

void
Robot::update_Q_mat(){
    Q_.resize(4 , 4, 0.0 );
    Q_(0, 0) = 0.1*0.1;
    Q_(1, 1) = 0.1*0.1;
    Q_(2, 2) = kernel::AngleCalculator::deg_to_rad(1.0)*kernel::AngleCalculator::deg_to_rad(1.0);;
    Q_(3, 3) = 1.0;
}

void
Robot::update_L_mat(){
    L_ = std::move(cengine::IdentityMatrix<real_t>(4));
}

void
Robot::update_H_mat(){
    H_.resize(2,4);
    H_(0,0) = 1.0;
    H_(0,1) = 0.0;
    H_(0,2) = 0.0;
    H_(0,3) = 0.0;

    H_(1,0) = 0.0;
    H_(1,1) = 1.0;
    H_(1,2) = 0.0;
    H_(1,3) = 0.0;
}

void
Robot::update_Hjac_mat(){

    Hjac_.resize(2,4);
    Hjac_(0,0) = 1.0;
    Hjac_(0,1) = 0.0;
    Hjac_(0,2) = 0.0;
    Hjac_(0,3) = 0.0;

    Hjac_(1,0) = 0.0;
    Hjac_(1,1) = 1.0;
    Hjac_(1,2) = 0.0;
    Hjac_(1,3) = 0.0;

}

void
Robot::update_M_mat(){
    M_ = std::move(cengine::IdentityMatrix<real_t>(2));
}

void
Robot::update_R_mat(){
   R_ = std::move(cengine::IdentityMatrix<real_t>(2));
}

void
Robot::update_B_mat(){

    if(B_.rows() == 0){
        B_.resize(4 , 2);
        B_(0, 1) = 0.0;
        B_(1, 0) = 0.0;
        B_(1, 1) = 0.0;
        B_(2, 0) = 0.0;
        B_(2, 1) = DT;
        B_(3, 0 ) = 1.0;
        B_(3, 1 ) = 0.0;
    }

    real_t phi = state_[2];
    B_(0, 0) = std::cos(phi)*DT;
    B_(1, 0) = std::sin(phi)*DT;
}

```
## <a name="m_func"></a> The main function
Here is the main function that drives the simulation. 


```cpp
int main(int argc, char** argv) {

    using namespace exe2;
    uint_t n_steps = 500;

    DynVec<real_t> u(2);
    u[0] = 1.0; //m/s
    u[1] = 0.1; //rad/s

    Robot robot;
    robot.set_input(u);
    robot.initialize();

    kernel::CSVWriter writer("robot_state", kernel::CSVWriter::default_delimiter(), true);
    std::vector<std::string> names(8);
    names[0] = "X";
    names[1] = "Y";
    names[2] = "X_true";
    names[3] = "Y_true";
    names[4] = "Phi";
    names[5] = "V";
    names[6] = "Zx";
    names[7] = "Zy";

    writer.write_column_names(names);

    try{

        for(uint_t step=0; step < n_steps; ++step){

            std::cout<<"\tAt step: "<<step<<std::endl;
            observation(u); // update y and ud
            robot.simulate(ud, y);
            //robot.simulate(u, y);
            robot.save_state(writer);
        }
    }
    catch(std::runtime_error& e){
        std::cerr<<e.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }
   
  
    return 0;
}

```
## <a name="results"></a> Results
A file named  robot_state.csv should exist in the directory you started the simulation. 
We can use Python plotting tools to visualize the robot's path. 



