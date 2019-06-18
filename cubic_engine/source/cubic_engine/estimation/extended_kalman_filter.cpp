#include "cubic_engine/estimation/extended_kalman_filter.h"
#include "cubic_engine/base/config.h"

namespace cengine
{

void
EKF_F_func::operator()(DynVec<real_t>& x, const DynVec<real_t>& x_prev, const DynMat<real_t>& A,
                       const DynMat<real_t>& B, const DynVec<real_t>& u  )const{
    x = A*x_prev + B*u;
}

DynVec<real_t>
EKF_H_func::operator()(const DynVec<real_t>& x, const DynMat<real_t>& H)const{
        return H*x;
}

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

    auto& Q = *this->system_maps_["Q"];
    auto& L = *this->system_maps_["L"];
    auto L_T = trans(L);

    auto& F = *this->system_maps_["F"];
    auto F_T = trans(F);
    P = F*P*F_T + L*Q*L_T;
}

void
ExtendedKalmanFilter::update(const DynVec<real_t>& y){

    auto& H = *this->system_maps_["H"];
    auto H_T = trans(H);
    auto& P = *this->system_maps_["P"];
    auto& M = *this->system_maps_["M"];
    auto M_T = trans(M);
    auto& R = *this->system_maps_["R"];

    auto S = H*P*H_T + M*R*M_T;
    auto S_inv = inv(S);

    this->K_ = std::move(P*H_T*S_inv);

    auto& x = *this->x_;
    auto innovation = y - h_ptr_->operator()(x, H);

#ifdef KERNEL_DEBUG

    if(this->K_.columns() != innovation.size()){
        throw std::runtime_error("Matrix columns: "+std::to_string(this->K_.columns())+" not equal to vector size: "+std::to_string(innovation.size()));
    }

#endif

    x += this->K_*innovation;

    IdentityMatrix<real_t> I(this->K_.rows());

    // update the covariance matrix
    P =  (I - this->K_*H)*P;

}

void
ExtendedKalmanFilter::assert_matrix_name_(const std::string& name)const{

    // TODO: fix the names to bettern match literature
    if( name != "Q" && name != "R" && name != "F" &&
        name != "P" && name != "A" && name != "B" && name != "Hjac" &&
        name != "K" && name != "H" && name != "L" && name != "M"){

        throw  std::invalid_argument("Matrix name "+name+" not in []");
    }
}

}
