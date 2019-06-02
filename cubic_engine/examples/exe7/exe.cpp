/**
 * This example demonstrates how to use Kalman filtering
 */

#include "robosim/systems/linear_steady_system.h"
#include "robosim/algorithms/kalman_filter.h"
#include "robosim/simulator/simulator_base.h"

#include <iostream>

using namespace robosim;
        
int main(){
    
    //initialize constants etc
    SimBase::init();
    
		//how many steps to perform
    const size_type nsteps = 3;

    //the linear system we model
    LinearSteadySys sys;
    
    //initialize the state vector
    sys.get_x_vec() = {0.0,10.0};
    
    //initialize the state transition matrix
    sys.get_A_mat()={{1.0,1.0},{0.0,1.0}};
    
    //initialize the error covariance matrix
    //associated with the state vector
    sys.get_P_mat()={{2.0,0.0},{0.0,3.0}};
    
    //initialize the coefficient matrix associated
    //with the noise error of the state
    auto& wmat = sys.get_W_mat();
    wmat.resize(2,1);
    wmat(0,0)=0.0;
    wmat(1,0)=1.0;
    
    //initialize the covariance matrix of the noise
    //associated with the state vector
    sys.get_Q_mat()={1.0};
    
    //initialize the coefficient matrix associated with
    //the control input
    sys.get_B_mat()={{0.0,0.0},{0.0,0.0}};
    
    //initialize the control input vector
    sys.get_u_vec() = {0.0,0.0};
    
    //initialize the the covariance matrix associated
    //with error in the measurement vector
    sys.get_R_mat()={2.0};
    
    //initialize the coefficients matrix of the measurement vector
    sys.get_H_mat()={1.0,0.0};
    
    //the Kalman filter
    LinearKF<LinearSteadySys> kf;
    
#ifdef ROBOSIM_DEBUG
    kf.set_show_iterations_flag(true);
#endif
    
    for(size_type step=0; step<nsteps; ++step){
        
        std::cout<<"At iteration: "<<step<<std::endl;
        
        //update the measurement vector
        if(step == 0){
            sys.get_z_vec()={9.0};
        }
        else if(step == 1){
           sys.get_z_vec()={19.5};    
        }
        else{
           sys.get_z_vec()={29.0};  
        }
        
        kf.step(sys);   
    }
    
    //finalize
    SimBase::finalize();
    
    return 0;
}

