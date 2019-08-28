/// \details
/// Demostration of simple linear Kalman Filter

#include "exe.h"
#include "cubic_engine/base/cubic_engine_types.h"

#include <iostream>

namespace exe
{

using namespace cengine;

using state_type = DynVec<real_t>;
struct Input
{
    typedef DynVec<real_t> control_type;
    typedef DynVec<real_t> measurement_type;
    // the measurement vector
    DynVec<real_t> y;

    //the input vector
    DynVec<real_t> u;

    const DynVec<real_t>& get_control()const{return u;}
    const DynVec<real_t>& get_measurement()const{return y;}
};

}

int main(int argc, char** argv) {

    using namespace cengine;

    std::cout<<"Running Example"<<std::endl;

    exe::state_type state;
    exe::Input input;

     {
        // we can do predict and update in one step
        // The Kalman filter class to use.
        KalmanFilter<exe::state_type, exe::Input, cengine::KalmanFilterMatrixDescriptor> kf(&state);
        kf.iterate(input);
     }
    return 0;
}

