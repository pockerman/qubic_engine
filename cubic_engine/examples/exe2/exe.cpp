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
Robot::update_A_mat(){
     A_.resize(4, 4);
     A_( 0,0 ) = 1.0;
     A_( 1,1 ) = 1.0;
     A_( 2,2 ) = 1.0;
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

    // set the state we want to estimate
    state_estimator_.set_state_vector_ptr(state_);
    state_estimator_.set_mat_ptr("A", A_);

}

template<typename... Args>
void
Robot::simulate(const Args& ... args){


}

}

int main(int argc, char** argv) {
   
    using namespace exe2;
    uint_t n_steps = 1000;
    real_t dt = 0.1;

    Robot robot;

    try{
        
        for(uint_t step=1; step < n_steps; ++step){

            std::cout<<"\tAt step: "<<step<<std::endl;
            robot.simulate(dt);
        }

    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }
   
  
    return 0;
}

