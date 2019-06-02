/**
 * This example demonstrates how to use Kalma filter
 */
#include "robosim/algorithms/extended_kalman_filter.h"
#include "robosim/systems/non_linear_system.h"
#include "robosim/simulator/simulator_base.h"

#include <iostream>
#include <cmath>

using namespace robosim;

class AC: public NonLinSys
{
    
public:
    
    //constructor
    AC();
    
    
    //Make any updates to the system
    virtual void update_AWQ_mats();
    
    //Update the state vector
    virtual void update_x_vec();
    
    //Update the measurements vector
    virtual void update_z_vec();
    
    //Update the H, V, R matrices of the system
    virtual void update_HVR_mats();
    
    //set up the AC matrices
    void ac_setup();
    
private:
    
    //update the state matrix A
    void update_A_mat();
    
    //update the W matrix
    void update_W_mat();
    
    //update the Q matrix
    void update_Q_mat();
    
    //update the H matrix
    void update_H_mat();
    
    //update the R matrix
    void update_R_mat();
    
    real_type period_;
    real_type mass_;
    real_type bfriction_;
    real_type portance_;
    real_type gravity_;
    
    
};

AC::AC()
:
NonLinSys(),
period_(0.2),
mass_(1000.),
bfriction_(0.35),
portance_(3.92),
gravity_(9.8)
{}

void
AC::update_AWQ_mats(){
    
    update_A_mat();
    update_W_mat();
    update_Q_mat();
}

void 
AC::update_HVR_mats(){
   update_H_mat();
   update_R_mat();
    
}

void
AC::update_A_mat(){
    
    auto& A = this->get_A_mat();
    auto& x = this->get_x_vec();
    
    A(0,0) = 1.0;
    A(0,1) = period_ - period_*period_*bfriction_/mass_*x(1);
    A(0,2) = 0.0;
    A(0,3) = 0.0;

    A(1,0) = 0.0;
    A(1,1) = 1. - 2*period_*bfriction_/mass_*x(1);
    A(1,2) = 0.0;
    A(1,3) = 0.0;

    A(2,0) = 0.0;
    A(2,1) = period_*period_*portance_/mass_*x(1);
    A(2,2) = 1.0;
    A(2,3) = period_;

    A(3,0) = 0.0;
    A(3,1) = 2*period_*portance_/mass_*x(1);
    A(3,2) = 0.0;
    A(3,3) = 1.0;
}

void
AC::update_W_mat(){
    
    auto& W = this->get_W_mat();
    
    W(0,0) = 0.0;
    W(0,1) = 0.0;
    W(1,0) = 1.0;
    W(1,1) = 0.0;
    W(2,0) = 0.0;
    W(2,1) = 0.0;
    W(3,0) = 0.0;
    W(3,1) = 1.0;
}

void
AC::update_Q_mat(){
    
    auto& Q = this->get_Q_mat();
    
    Q(0,0) = 0.01*0.01;
    Q(0,1) = 0.01*0.01/10.0;
    Q(1,0) = 0.01*0.01/10.0;
    Q(1,1) = 0.01*0.01;
}

void
AC::update_H_mat(){
    
    auto& H = this->get_H_mat();
    auto& x = this->get_x_vec();
    
    H(0,0) = -x(2)/(x(0)*x(0)+x(2)*x(2));
    H(0,1) = 0.0;
    H(0,2) = x(0)/(x(0)*x(0)+x(2)*x(2));
    H(0,3) = 0.0;

    H(1,0) = x(0)/std::sqrt(x(0)*x(0)+x(2)*x(2));
    H(1,1) = 0.0;
    H(1,2) = x(2)/std::sqrt(x(0)*x(0)+x(2)*x(2));
    H(1,3) = 0.0;
}

void
AC::update_R_mat(){
    
    auto& R = this->get_R_mat();
    
    R(0,0) = 0.01*0.01;
    R(0,1) = 0.01*0.01/10.0;
    R(1,0) = 0.01*0.01/10.0;
    R(1,1) = 0.01*0.01;
}

void 
AC::update_x_vec(){
    
    auto tmpx = this->get_x_vec();
    auto& x = this->get_x_vec();
    auto& u = this->get_u_vec();
    
    tmpx(0) = x(0) + x(3)*period_ + (period_*period_)/2*(u(0)/mass_ - bfriction_/mass_*x(1)*x(1));
    tmpx(1) = x(1) + (u(0)/mass_ - bfriction_/mass_*x(1)*x(1))*period_;
    tmpx(2) = x(2) + x(3)*period_ + (period_*period_)/2*(portance_/mass_*x(1)*x(1)-gravity_);
    tmpx(3) = x(3) + (portance_/mass_*x(1)*x(1)-gravity_)*period_;
    
    x = tmpx;
}

void 
AC::update_z_vec(){ 
    auto& z = this->get_z_vec();
    z(0)= std::atan2(x(2), x(0));
    z(1)= std::sqrt(x(0)*x(0)+x(2)*x(2));
}

void 
AC::ac_setup(){ 
    
    const size_type n = 4;
    const size_type m = 2;
    const size_type p = 2;
    const size_type l = 2;
    
    this->initialize(n,m,p,l);
    this->set_P_matrix({{100.0*100.0, 0.0, 0.0, 0.0},{0.0, 10.0*10.0, 0.0, 0.0},
                        {0.0, 0.0, 25.0*25.0, 0.0},{0.0, 0.0, 0.0, 10.0*10.0}});
                        
                        
    
}
        
int main(){
    
    //initialize constants etc
    SimBase::init();
    
    const size_type nsteps = 100;
    
    //the system
    AC sys;
    
    //set up the AC
    sys.ac_setup();

    //the filter we use
    EKF kf;
      
#ifdef ROBOSIM_DEBUG
    kf.set_show_iterations_flag(true);
#endif
    
    for(size_type step=0; step<nsteps; ++step){
        
        std::cout<<"At iteration: "<<step<<std::endl;
        kf.step(sys);   
    }
    
    //finalize
    SimBase::finalize();
    
    return 0;
}

