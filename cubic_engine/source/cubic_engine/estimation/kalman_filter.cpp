#include "cubic_engine/estimation/kalman_filter.h"

namespace cengine
{

    KalmanFilter::KalmanFilter(DynVec<real_t>* x)
        :
         system_maps_(),
         K_(),
         x_(x)
    {

        std::string names[] = {"A", "B", "H", "P"};

        for(auto name:names){
            system_maps_[name] = nullptr;
        }
    }

    KalmanFilter::~KalmanFilter()
    {}

    void
    KalmanFilter::predict(const DynVec<real_t>& u){

        if(!x_){
            throw std::runtime_error("state vector has not been assigned");
        }

        auto& A = *system_maps_["A"];
        auto& B = *system_maps_["B"];

        //predict the state vector
        *x_ = A*(*x_) + B*u;

        //predict the covariance matrix
        auto& P = *system_maps_["P"];
        auto& Q = *system_maps_["Q"];
        auto A_T = trans( A );

        P = (A*(P*A_T)) + Q;

    }

    void
    KalmanFilter::update(const DynVec<real_t>& y){

        auto& H = *system_maps_["H"];
        auto& P = *system_maps_["P"];
        auto& R = *system_maps_["R"];
        auto H_T = trans(H);

        auto S = H*P*H_T + R;
        auto S_inv = inv(S);

        // compute gain matrix
        auto& K = *system_maps_["K"];

        K = P*H_T*S_inv;

        auto& x = *x_;
        auto innovation = y - H*x;

        x += K*innovation;

        IdentityMatrix<real_t> I(K.rows());

        // update covariance matrix
        P = (I - K*H)*P;
    }

    void
    KalmanFilter::assert_matrix_name_(const std::string& name)const{

        if( name != "Q" && name != "R" &&
            name != "P" && name != "A" && name != "B" &&
            name != "K" && name != "H" ){

            throw  std::invalid_argument("Matrix name "+name+" not in []");
        }
    }

}
