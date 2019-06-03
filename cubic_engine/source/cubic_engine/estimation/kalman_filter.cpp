#include "cubic_engine/estimation/kalman_filter.h"

namespace cengine
{

    KalmanFilter::KalmanFilter()
        :
         system_maps_(),
         K_()
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


    }

    void
    KalmanFilter::update(const DynVec<real_t>& y){

    }

}
