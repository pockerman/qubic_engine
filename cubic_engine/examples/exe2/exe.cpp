/***
 * Extended Kalman Filter
*/


#include "exe.h"
#include "parframe/base/angle_calculator.h"
#include "parframe/utilities/csv_file_writer.h"

#include <boost/math/distributions/normal.hpp> // normal distribution
#include <cmath>
#include <iostream>
#include <tuple>
#include <random>

namespace exe2
{


DynVec<real_t> z(2, 0.0);
DynVec<real_t> x_true(4, 0.0);
DynVec<real_t> ud(2, 0.0);


struct MotionModel
{
    DynVec<real_t> operator()(const DynVec<real_t>& x, const DynVec<real_t>& u)const;
};

DynVec<real_t>
MotionModel::operator()(const DynVec<real_t>& x, const DynVec<real_t>& u)const{

    DynMat<real_t> A(4, 4, 0.0);

    A( 0,0 ) = 1.0;
    A( 1,1 ) = 1.0;
    A( 2,2 ) = 1.0;

    DynMat<real_t> B(4, 2, 0.0);
    real_t phi = x[2];

    B(0, 0) = std::cos(phi)*DT;
    B(0, 1) = 0.0;
    B(1, 0) = std::sin(phi)*DT;
    B(1, 1) = 0.0;
    B(2, 0) = 0.0;
    B(2, 1) = DT;
    B(3, 0 ) = 1.0;
    B(3, 1 ) = 0.0;

    return A*x + B*u;
}

struct ObservationModel
{
    DynVec<real_t> operator()(const DynVec<real_t>& x)const;
};

DynVec<real_t>
ObservationModel::operator()(const DynVec<real_t>& x)const{

    DynMat<real_t> H(2, 4, 0.0);

#ifdef KERNEL_DEBUG

    if(H.columns() != x.size()){
        throw std::runtime_error("Matrix columns: "+std::to_string(H.columns())+" not equal to vector size: "+std::to_string(x.size()));
    }
#endif

    H(0,0) = 1.0;
    H(0,1) = 0.0;
    H(0,2) = 0.0;
    H(0,3) = 0.0;

    H(1,0) = 0.0;
    H(1,1) = 1.0;
    H(1,2) = 0.0;
    H(1,3) = 0.0;

    return H*x;
}

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
    P_ = cengine::IdentityMatrix<real_t>(4);
}

void
Robot::update_Q_mat(){
    Q_.resize(4 , 4, 0.0 );
    Q_(0, 0) = 0.1*0.1;
    Q_(1, 1) = 0.1*0.1;
    Q_(2, 2) = kernel::AngleCalculator::deg_to_rad(1.0)*kernel::AngleCalculator::deg_to_rad(1.0);
    Q_(3, 3) = 1.0;
}

void
Robot::update_L_mat(){
    L_ = cengine::IdentityMatrix<real_t>(4);
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
    M_ = cengine::IdentityMatrix<real_t>(2);
}

void
Robot::update_R_mat(){
   R_ = cengine::IdentityMatrix<real_t>(2);
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

void
Robot::initialize(){

    // the state has 4 variables
    state_  = DynVec<real_t>(4, 0.0);
    state_estimator_.set_state_vector_ptr(state_);

    update_A_mat();
    update_F_mat();
    update_B_mat();
    update_P_mat();
    update_Q_mat();
    update_L_mat();
    update_H_mat();
    update_Hjac_mat();
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
    state_estimator_.set_mat_ptr("Hjac", Hjac_);

    state_estimator_.set_motion_model(motion_model_);
    state_estimator_.set_observation_model(h_func_);
}

void
Robot::simulate(DynVec<real_t>& u, const DynVec<real_t>& y){

    u_ = &u;
    state_estimator_.iterate(u, y);

    std::cout<<"\t Estimated state: "<<std::endl;
    std::cout<<"\t\t"<<state_[0]<<", "<<state_[1]<<", "<<state_[2]<<", "<<state_[3]<<std::endl;

    update_F_mat();
    update_B_mat();
}

void
Robot::apply_motion_model(DynVec<real_t>& x, const DynVec<real_t>& u)const{
    motion_model_(x, x, A_, B_, u);
}

void
Robot::save_state(kernel::CSVWriter& writer)const{

    DynVec<real_t> x(state_.size() + 4);

    x[0] = state_[0];
    x[1] = state_[1];
    x[2] = x_true[0];
    x[3] = x_true[1];
    x[4] = state_[2];
    x[5] = state_[3];
    x[6] = z[0];
    x[7] = z[1];
    writer.write_row(x);
}

void
observation( const  DynVec<real_t>& u){

    MotionModel mmodel;
    x_true = mmodel(x_true, u);

    std::cout<<"\t True state: "<<std::endl;
    std::cout<<"\t\t"<<x_true[0]<<", "<<x_true[1]<<", "<<x_true[2]<<", "<<x_true[3]<<std::endl;

    std::default_random_engine generator;
    std::normal_distribution<real_t> y_dist(0.0, 0.25);

    // add noise to gps x-y
    z[0] = x_true[0] + y_dist(generator);
    z[1] = x_true[1] + y_dist(generator);

    std::cout<<"\t Measured position: "<<std::endl;
    std::cout<<"\t\t"<<z[0]<<", "<<z[1]<<std::endl;

    std::normal_distribution<real_t> u_dist_1(0.0, 1.0);
    std::normal_distribution<real_t> u_dist_2(0.0,kernel::AngleCalculator::deg_to_rad(30.0) * kernel::AngleCalculator::deg_to_rad(30.0));

    ud[0] = u[0] + u_dist_1(generator);
    ud[1] = u[1] + u_dist_2(generator);

    std::cout<<"\t Control input: "<<std::endl;
    std::cout<<"\t\t"<<ud[0]<<", "<<ud[1]<<std::endl;
}
}

int main(int argc, char** argv) {
   
    using namespace exe2;
    uint_t n_steps = N_STEPS;

    DynVec<real_t> u(2);
    u[0] = 1.0; //m/s
    u[1] = 0.1; //rad/s

    Robot robot;
    robot.set_input(u);
    robot.initialize();

    kernel::CSVWriter writer("robot_state", kernel::CSVWriter::default_delimiter(), true);
    std::vector<std::string> names{"X", "Y", "X_true", "Y_true", "Phi", "V", "Zx", "Zy" };
    writer.write_column_names(names);

    try{

        for(uint_t step=0; step < n_steps; ++step){

            std::cout<<"At step: "<<step<<std::endl;
            observation(u); // update y and ud
            robot.simulate(ud, z);
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

