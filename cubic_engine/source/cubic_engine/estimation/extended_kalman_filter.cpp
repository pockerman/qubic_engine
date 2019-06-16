#include "cubic_engine/estimation/extended_kalman_filter.h"

namespace cengine
{

ExtendedKalmanFilter::ExtendedKalmanFilter(DynVec<real_t>* x)
    :
    KalmanFilter (x),
    f_ptr_(nullptr),
    h_ptr_(nullptr)
{}

ExtendedKalmanFilter::~ExtendedKalmanFilter()
{}

void
ExtendedKalmanFilter::predict(const DynVec<real_t>& u){

    auto& A = *this->system_maps_["A"];

    // update the prediction of the state vector
    f_ptr_->operator()(*this->x_, *this->x_,  A, *this->system_maps_["B"], u);

    // update the the covariance matrix
    auto& P = *this->system_maps_["P"];
    auto A_T = trans(A);
    auto& Q = *this->system_maps_["Q"];
    auto& L = *this->system_maps_["L"];
    auto L_T = trans(L);
    P = A*P*A_T + L*Q*L_T;
}

void
ExtendedKalmanFilter::update(const DynVec<real_t>& y){

    auto& H = *this->system_maps_["H"];
    auto H_T = inv(H);
    auto& P = *this->system_maps_["P"];
    auto& M = *this->system_maps_["M"];
    auto M_T = trans(M);
    auto& R = *this->system_maps_["R"];

    auto S = H*P*H_T + M*R*M_T;
    auto S_inv = inv(S);

    auto& K = *this->system_maps_["K"];
    K = P*H_T*S_inv;

    auto& x = *this->x_;
    auto innovation = y - h_ptr_->operator()(x);
    x += K*innovation;

    IdentityMatrix<real_t> I(K.rows());

    // update the covariance matrix
    P =  (I - K*H)*P;

}

void
ExtendedKalmanFilter::assert_matrix_name_(const std::string& name)const{

    if( name != "Q" && name != "R" &&
        name != "P" && name != "A" && name != "B" &&
        name != "K" && name != "H" && name != "L"){

        throw  std::invalid_argument("Matrix name "+name+" not in []");
    }
}

}
