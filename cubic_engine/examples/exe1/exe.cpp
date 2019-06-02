/// \details
/// Demostration of simple linear Kalman Filter




#include "exe.h"
#include "cubic_engine/base/cubic_engine_types.h"

#include <iostream>

int main(int argc, char** argv) {

    using namespace cengine;

    std::cout<<"Running Example"<<std::endl;

    // the measurement vector
    DynVec<real_t> y;

    //the input vector
    DynVec<real_t> u;
    
    {

        // The Kalman filter class to use.
        KalmanFilter kf;
        kf.predict(u);
        kf.update(y);
     }

     {
        // we can do predict and update in one step
        // The Kalman filter class to use.
        KalmanFilter kf;
        kf.iterate(u,y);
     }
    return 0;
}

