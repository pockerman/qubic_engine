#include "ekf_data.h"
#include "kernel/base/angle_calculator.h"
#include <cmath>

namespace exe
{

void update_A_mat(){

    A.resize(4, 4, 0.0);
    A( 0, 0 ) = 1.0;
    A( 1, 1 ) = 1.0;
    A( 2, 2 ) = 1.0;
}

void update_F_mat(){

    F.resize(4, 4);

    real_t yaw = state[2];
    real_t v = u[0];

    F(0, 0) = 1.0;
    F(0, 1 ) = 0.0;
    F(0, 2) = -DT * v * std::sin(yaw);
    F(0, 3) =  DT * std::cos(yaw);

    F(1, 0) = 0.0;
    F(1, 1) = 1.0;
    F(1, 2) = DT * v * std::cos(yaw);
    F(1,3 ) = DT * std::sin(yaw);

    F(2, 0 ) = 0.0;
    F(2, 1 ) = 0.0;
    F(2, 2 ) = 1.0;
    F(2, 3 ) = 0.0;

    F(3, 0 ) = 0.0;
    F(3, 1 ) = 0.0;
    F(3, 2 ) = 0.0;
    F(3, 3 ) = 1.0;
}

void update_P_mat(){
    P = std::move(cengine::IdentityMatrix<real_t>(4));
}

void update_Q_mat(){
    Q.resize(4 , 4, 0.0 );
    Q(0, 0) = 0.1*0.1;
    Q(1, 1) = 0.1*0.1;
    Q(2, 2) = kernel::AngleCalculator::deg_to_rad(1.0)*kernel::AngleCalculator::deg_to_rad(1.0);;
    Q(3, 3) = 1.0;
}

void update_L_mat(){
    L = std::move(cengine::IdentityMatrix<real_t>(4));
}

void update_H_mat(){

    H.resize(2,4);
    H(0,0) = 1.0;
    H(0,1) = 0.0;
    H(0,2) = 0.0;
    H(0,3) = 0.0;

    H(1,0) = 0.0;
    H(1,1) = 1.0;
    H(1,2) = 0.0;
    H(1,3) = 0.0;
}

void update_Hjac_mat(){

    Hjac.resize(2,4);
    Hjac(0,0) = 1.0;
    Hjac(0,1) = 0.0;
    Hjac(0,2) = 0.0;
    Hjac(0,3) = 0.0;

    Hjac(1,0) = 0.0;
    Hjac(1,1) = 1.0;
    Hjac(1,2) = 0.0;
    Hjac(1,3) = 0.0;
}

void update_M_mat(){
    M = std::move(cengine::IdentityMatrix<real_t>(2));
}

void update_R_mat(){
   R = std::move(cengine::IdentityMatrix<real_t>(2));
}

void update_B_mat(){

    if(B.rows() == 0){
        B.resize(4 , 2);
        B(0, 1) = 0.0;
        B(1, 0) = 0.0;
        B(1, 1) = 0.0;
        B(2, 0) = 0.0;
        B(2, 1) = DT;
        B(3, 0 ) = 1.0;
        B(3, 1 ) = 0.0;
    }

    real_t phi = state[2];
    B(0, 0) = std::cos(phi)*DT;
    B(1, 0) = std::sin(phi)*DT;
}

void initialize_ekf(){

    /*u[0] = 1.0;
    u[1] = 0.1;

    state_estimator.set_state_vector_ptr(state);

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

    state_estimator.set_mat_ptr("A", A);
    state_estimator.set_mat_ptr("F", F);
    state_estimator.set_mat_ptr("B", B);
    state_estimator.set_mat_ptr("P", P);
    state_estimator.set_mat_ptr("Q", Q);
    state_estimator.set_mat_ptr("L", L);
    state_estimator.set_mat_ptr("H", H);
    state_estimator.set_mat_ptr("M", M);
    state_estimator.set_mat_ptr("R", R);
    state_estimator.set_mat_ptr("Hjac", Hjac);

    state_estimator.set_motion_model(motion_model);
    state_estimator.set_observation_model(h_func);*/
}



}
