#include "cubic_engine/control/pid_controller.h"

namespace cengine
{

PIDControl::PIDControl(real_t Kp, real_t Kd, real_t Ki)
    :
    parameters_(),
    error_(0.0)
{
    parameters_["Kp"] = Kp;
    parameters_["Kd"] = Kd;
    parameters_["Ki"] = Ki;
}

void
PIDControl::set_paramter(const std::string& name, real_t val){

    auto itr = parameters_.find(name);

    if(itr != parameters_.end() ){

        itr->second = val;
    }
    else{
        throw std::runtime_error("Name: "+name+" not in ['Kp', 'Kd', 'Ki' ]");
    }
}

real_t
PIDControl::execute(real_t error, real_t dt){

    auto rslt = 0.0;
    auto delta_error = error - error_;
    rslt += parameters_["Kd"] * (delta_error/dt);
    rslt += parameters_["Kp"] * error;

    //accummulate the error
    error_ += error;
    rslt += parameters_["Ki"] * error_;

    return rslt;
}

}
