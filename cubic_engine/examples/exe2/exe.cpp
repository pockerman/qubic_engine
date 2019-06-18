/***
 * Extended Kalman Filter
*/


#include "exe.h"
#include "parframe/base/angle_calculator.h"
#include <cmath>
#include <iostream>

namespace exe2
{

/*void
MotionModel::operator()(DynVec<real_t>& x, const DynVec<real_t>& x_prev,
                        const DynMat<real_t>& A, const DynMat<real_t>& B, const DynVec<real_t>& u  )const{
    x = A*x_prev + B*u;
}

DynVec<real_t>
ObservationModel::operator()(const DynVec<real_t>& x)const{

    return DynVec<real_t>(2,0.0);
}*/

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
      f_func_(),
      h_func_()
{}

void
Robot::update_A_mat(){
     A_.resize(4, 4);
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
    F_(0,1 ) = 0.0;
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

    P_.resize(4, 4);
    P_( 0,0 ) = 0.0;
    P_( 1,1 ) = 0.0;
    P_( 2,2 ) = 0.0;
}

void
Robot::update_Q_mat(){
    Q_.resize(4 , 4 );
    Q_(0, 0) = 0.1*0.1;
    Q_(1, 1) = 0.1*0.1;
    Q_(2, 2) = kernel::AngleCalculator::deg_to_rad(1.0);
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
        B_(2, 1) = DT;
        B_( 3, 0 ) = 1.0;
    }

    real_t phi = state_[2];
    B_(0,0) = std::cos(phi)*DT;
    B_(1,0) = std::sin(phi)*DT;
}

void
Robot::initialize(){

    // the state has 4 variables
    state_.resize(4);
    state_estimator_.set_state_vector_ptr(state_);

    update_A_mat();
    update_F_mat();
    update_B_mat();
    update_P_mat();
    update_Q_mat();
    update_L_mat();
    update_H_mat();
    update_M_mat();
    update_R_mat();

    state_estimator_.set_mat_ptr("A", A_);
    state_estimator_.set_mat_ptr("F", F_);
    state_estimator_.set_mat_ptr("B", B_);
    state_estimator_.set_mat_ptr("P", P_);
    state_estimator_.set_mat_ptr("Q", Q_);
    state_estimator_.set_mat_ptr("L", L_);
    state_estimator_.set_mat_ptr("H", H_);
    state_estimator_.set_mat_ptr("M", M_);
    state_estimator_.set_mat_ptr("R", R_);

    state_estimator_.set_motion_model(f_func_);
    state_estimator_.set_observation_model(h_func_);
}

void
Robot::simulate(DynVec<real_t>& u, const DynVec<real_t>& y){

    state_estimator_.iterate(u, y);

}

}

int main(int argc, char** argv) {
   
    using namespace exe2;
    uint_t n_steps = 1000;
    real_t dt = 0.1;

    Robot robot;
    robot.initialize();

    DynVec<real_t> u(2);
    u[0] = 2.0; //m/s
    u[1] = 0.1; //rad/s

    DynVec<real_t> y(2, 0.0);

    try{
        for(uint_t step=1; step < n_steps; ++step){
            std::cout<<"\tAt step: "<<step<<std::endl;
            robot.simulate(u, y);
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

