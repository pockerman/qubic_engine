/***
 * Extended Kalman Filter
 *
 *
 *
*/


#include "exe.h"

#include <cmath>
#include <iostream>

namespace exe2
{

template<typename... Args>
void
Dynamics::execute(const Args&... args){


}

void
MotionModel::operator()(DynVec<real_t>& x, const DynVec<real_t>& x_prev, const DynMat<real_t>& A,
                        const DynMat<real_t>& B, const DynVec<real_t>& u  )const{


}

DynVec<real_t>
ObservationModel::operator()(const DynVec<real_t>& x)const{

    return DynVec<real_t>(x);
}

void
Robot::update_A_mat(){
     A_.resize(4, 4);
     A_( 0,0 ) = 1.0;
     A_( 1,1 ) = 1.0;
     A_( 2,2 ) = 1.0;
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
    Q_(2, 2) = 0.1*0.1;
    Q_(3, 3) = 1.0;

}

void
Robot::update_L_mat(){

    L_ = std::move(cengine::IdentityMatrix<real_t>(4));
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

    update_A_mat();
    update_B_mat();
    update_P_mat();
    update_Q_mat();
    update_L_mat();

    // set the state we want to estimate
    state_estimator_.set_state_vector_ptr(state_);
    state_estimator_.set_mat_ptr("A", A_);
    state_estimator_.set_mat_ptr("B", B_);
    state_estimator_.set_mat_ptr("P", P_);
    state_estimator_.set_mat_ptr("Q", Q_);
    state_estimator_.set_mat_ptr("L", L_);
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

    try{
        for(uint_t step=1; step < n_steps; ++step){
            std::cout<<"\tAt step: "<<step<<std::endl;
            robot.simulate(u, u);
        }

    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }
   
  
    return 0;
}

