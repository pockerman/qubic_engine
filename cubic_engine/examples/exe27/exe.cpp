#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/estimation/unscented_kalman_filter.h"
#include "kernel/dynamics/diff_drive_dynamics.h"
#include "kernel/base/angle_calculator.h"
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
using cengine::estimation::UnscentedKalmanFilter;
using cengine::DynMat;
using cengine::DynVec;
using kernel::dynamics::SysState;

const real_t TOL = 1.0e-8;
const real_t DT = 0.5;
const real_t STD_RADAR_RHO = 0.01;
const real_t STD_RADAR_THETA = 0.01;
const real_t STD_RADAR_RHO_D = 0.01;


class MotionModel
{
public:

    typedef DynMat<real_t> matrix_t;
    typedef SysState<5> state_t;
    typedef std::tuple<real_t, real_t, std::array<real_t, state_t::dimension>> input_t;

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
    SysState<5> state_;
};

MotionModel::MotionModel()
    :
      state_()
{
   state_.set(0, std::make_pair("X", 0.0));
   state_.set(1, std::make_pair("Y", 0.0));
   state_.set(2, std::make_pair("Theta", 0.0));
   state_.set(3, std::make_pair("V", 0.0));
   state_.set(4, std::make_pair("ThetaDot", 0.0));
}

DynVec<real_t>
MotionModel::evaluate(const DynVec<real_t>& vec,
                      const input_t& input){

    DynVec<real_t> result(vec.size(), 0.0);
    if(std::fabs(std::get<1>(input)) < TOL){
       real_t v = std::get<0>(input);
       result [0] = vec [0] + v*std::cos(state_["Theta"])*DT;
       result [1] = vec [1] + v*std::sin(state_["Theta"])*DT;
       result [2] = 0.0;
       result [3] = 0.0;
       result [4] = 0.0;

    }

    return result;
}


class ObservationModel
{

public:

    typedef  DynVec<real_t> input_t;

    ObservationModel();

    std::vector<DynVec<real_t>>
    evaluate(const std::vector<DynVec<real_t>>& sigma)const;

private:


};

ObservationModel::ObservationModel() 
{}

std::vector<DynVec<real_t>>
ObservationModel::evaluate(const std::vector<DynVec<real_t>>& sigma)const{

    std::vector<DynVec<real_t>> result(sigma.size());

    for(uint_t sp=0; sp<sigma.size(); ++sp){

        auto& vec = result[sp];
        vec.resize(3, 0.0);

        auto x = sigma[sp][0];
        auto y = sigma[sp][1];
        auto theta = sigma[sp][2];
        auto v = sigma[sp][3];

        vec[0] = std::sqrt(kernel::utils::sqr(x) +
                           kernel::utils::sqr(y));
        vec[1] = theta;
        vec[2] = (x*std::cos(theta)*v + y*std::sin(theta))/vec[0];
    }

    return result;
}

const DynVec<real_t> get_measurement(const SysState<3>& state){
   return DynVec<real_t>({state.get("X"), state.get("Y")});
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

    std::array<real_t, 5> motion_control_error;
    for(uint_t i=0; i<motion_control_error.size(); ++i){
        motion_control_error[i] = 0.0;
    }

    MotionModel motion_model;
    ObservationModel observation;

    UnscentedKalmanFilter<MotionModel, ObservationModel> ukf(motion_model, observation);

    DynMat<real_t> R(3, 3, 0.0);
    R(0,0) = STD_RADAR_RHO;
    R(1, 1) = STD_RADAR_THETA;
    R(2, 2) = STD_RADAR_RHO_D;

    DynMat<real_t> Q(MotionModel::state_t::dimension,
                     MotionModel::state_t::dimension, 0.0);
    //Q(0,0) = 0.001;
    //Q(1, 1) = 0.001;

    DynMat<real_t> P(MotionModel::state_t::dimension,
                     MotionModel::state_t::dimension, 0.0);

    for(uint_t p=0; p<P.rows(); ++p){
        P(p, p) = 1.0;
    }

    ukf.set_matrix("P", P);
    ukf.set_matrix("R", R);
    ukf.set_matrix("Q", Q);

    kernel::CSVWriter writer("state", kernel::CSVWriter::default_delimiter(), true);
    std::vector<std::string> names{"Time", "X", "Y"};
    writer.write_column_names(names);

    try{

        uint_t counter=0;
        for(uint_t step=0; step < n_steps; ++step){

            std::cout<<"At step: "<<step<<" time: "<<time<<std::endl;

            if(counter == 50){
              w = 0.0*kernel::UnitConverter::degrees_to_rad(45.0);
            }
            else if(counter == 100){
               w = 0.0*kernel::UnitConverter::degrees_to_rad(-45.0);
            }
            else if(counter == 150){
               w = 0.0*kernel::UnitConverter::degrees_to_rad(-45.0);
            }
            else{
                w = 0.0;
            }


            auto motion_input = std::make_tuple(vt, w, motion_control_error);
            //auto& exact_state = exact_motion_model.evaluate(motion_input);

            ukf.predict(motion_input);

            auto& state = motion_model.get_state();
            auto z = get_measurement(state);
            ukf.update(z);

            std::cout<<"Position: "<<ukf.get("X")<<", "<<ukf.get("Y")<<std::endl;
            std::cout<<"Orientation: "<<kernel::UnitConverter::rad_to_degrees(ukf.get("Theta"))<<std::endl;
            std::cout<<"V: "<<vt<<", W: "<<w<<std::endl;

            std::vector<real_t> row(3, 0.0);
            row[0] = time;
            row[1] = state.get("X");
            row[2] = state.get("Y");
            writer.write_row(row);

            time += DT;
            counter++;
        }
    }
    catch(std::runtime_error& e){
        std::cerr<<e.what()<<std::endl;
    }
    catch(std::logic_error& e){
        std::cerr<<e.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }
   
    return 0;
}

