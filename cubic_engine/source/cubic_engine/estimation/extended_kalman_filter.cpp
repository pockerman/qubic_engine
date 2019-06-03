#include "cubic_engine/estimation/extended_kalman_filter.h"

namespace cengine
{

ExtendedKalmanFilter::ExtendedKalmanFilter()
    :
      KalmanFilter ()
{}

ExtendedKalmanFilter::~ExtendedKalmanFilter()
{}

void
ExtendedKalmanFilter::predict(const DynVec<real_t>& u){

}

void
ExtendedKalmanFilter::update(const DynVec<real_t>& y){

}

}
