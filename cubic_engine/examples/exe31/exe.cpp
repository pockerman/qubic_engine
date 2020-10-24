#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/search/rapidly_exploring_random_tree.h"
#include "kernel/dynamics/diff_drive_dynamics.h"

#include "kernel/utilities/csv_file_writer.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/maths/constants.h"
#include "kernel/base/unit_converter.h"
#include "kernel/utilities/common_uitls.h"

#include <cmath>
#include <iostream>
#include <tuple>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using kernel::dynamics::DiffDriveDynamics;
using cengine::DynMat;
using cengine::DynVec;
using kernel::dynamics::SysState;
using cengine::search::RRT;

const real_t TOL = 1.0e-8;
const real_t DT = 0.5;
const real_t STD_RADAR_RHO = 0.01;
const real_t STD_RADAR_THETA = 0.01;
const real_t STD_RADAR_RHO_D = 0.01;

class MotionModel
{
public:

    typedef DynMat<real_t> matrix_t;
    typedef SysState<3> state_t;
    typedef std::tuple<real_t, real_t,
                       std::array<real_t, state_t::dimension>> input_t;

    /// constructor
    MotionModel();

    /// evaluate the motion model
    DynVec<real_t> evaluate(const DynVec<real_t>& vec,
                            const input_t& input);

    state_t& get_state(){return state_;}
    const state_t& get_state()const{return state_;}
    real_t get(const std::string& name)const{return state_.get(name);}

private:

    /// the state to track
    SysState<3> state_;
};

MotionModel::MotionModel()
    :
      state_()
{
   state_.set(0, {"X", 0.0});
   state_.set(1, {"Y", 0.0});
   state_.set(2, {"Theta", 0.0});
}

DynVec<real_t>
MotionModel::evaluate(const DynVec<real_t>& vec,
                      const input_t& input){

    DynVec<real_t> result(vec.size(), 0.0);
    if(std::fabs(std::get<1>(input)) < TOL){

       real_t v = std::get<0>(input);
       auto errors = std::get<2>(input);
       auto distance = v*DT;
       auto xincrement = (distance + errors[0])*std::cos(state_["Theta"]  + errors[1]);
       auto yincrement = (distance + errors[0])*std::sin(state_["Theta"]  + errors[1]);

       result [0] = vec [0] + xincrement;
       result [1] = vec [1] + yincrement;
       result [2] = vec [2];
    }
    else{
        real_t v = std::get<0>(input);
        real_t w = std::get<1>(input);
        auto errors = std::get<2>(input);

        result[0] = vec[0] + (v/w  + errors[0])*(std::sin(state_["Theta"] + w*DT) - std::sin(state_["Theta"]));
        result[1] = vec[1] - (v/w  + errors[0])*(-std::cos(state_["Theta"] + w*DT) + std::cos(state_["Theta"]));
        result[2] = vec[2] + w*DT + errors[1];

    }

    return result;
}

}

int main() {
   
    using namespace example;
    uint_t n_steps = 300;

    ///
    auto time = 0.0;

    /// angular velocity
    auto w = 0.0;

    /// linear velocity
    auto vt = 1.0;

    std::array<real_t, MotionModel::state_t::dimension> motion_control_error;
    for(uint_t i=0; i<motion_control_error.size(); ++i){
        motion_control_error[i] = 0.0;
    }

    MotionModel motion_model;

    DynMat<real_t> R(2, 2, 0.0);

    kernel::CSVWriter writer("state", kernel::CSVWriter::default_delimiter(), true);
    std::vector<std::string> names{"Time", "X", "Y"};
    writer.write_column_names(names);

    try{

        // create the grid

    }
    catch(std::runtime_error& e){
        std::cerr<<"Runtime error: "
                 <<e.what()<<std::endl;
    }
    catch(std::logic_error& e){
        std::cerr<<"Logic error: "
                 <<e.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }
   
    return 0;
}

