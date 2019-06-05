#include "cubic_engine/estimation/extended_kalman_filter.h"

namespace cengine
{

ExtendedKalmanFilter::ExtendedKalmanFilter(DynVec<real_t>* x)
    :
    KalmanFilter (x),
    f_ptr_(nullptr)
{}

ExtendedKalmanFilter::~ExtendedKalmanFilter()
{}

void
ExtendedKalmanFilter::predict(const DynVec<real_t>& u){

    auto& A = *this->system_maps_["A"];
    // update the prediction of the state vector
    f_ptr_.lock()->operator()(*this->x_, *this->x_,  A, *this->system_maps_["B"], u);

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

    auto innovation = y - self['h'](self['x'], 0.0)
                   self['x'] = self['x'] + np.dot(self['K'], innovation)
                   I = np.identity(self['K'].shape[0])
                   self['P'] = np.dot(I - np.dot(self['K'], self['H']), self['P'])

}

}
