#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/estimation/extended_kalman_filter.h"
#include "kernel/dynamics/diff_drive_dynamics.h"
#include "kernel/base/angle_calculator.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/maths/constants.h"
#include "kernel/base/unit_converter.h"

#include <cmath>
#include <iostream>
#include <tuple>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using kernel::dynamics::DiffDriveDynamics;
using cengine::ExtendedKalmanFilter;
using cengine::DynMat;
using cengine::DynVec;
using kernel::dynamics::SysState;


class ObservationModel
{

public:

    typedef  DynVec<real_t> input_t;

    ObservationModel();

    // simply return the state
    const DynVec<real_t> evaluate(const DynVec<real_t>& input)const;

    // get the H or M matrix
    const DynMat<real_t>& get_matrix(const std::string& name)const;

private:

    DynMat<real_t> H;
    DynMat<real_t> M;
};

ObservationModel::ObservationModel()
    :
      H(2,3, 0.0),
      M(2, 2, 0.0)
{
    H(0, 0) = 1.0;
    H(1,1) = 1.0;
    M(0,0) = 1.0;
    M(1, 1) = 1.0;

}

const DynVec<real_t>
ObservationModel::evaluate(const DynVec<real_t>& input)const{
    return input;
}

const DynMat<real_t>&
ObservationModel::get_matrix(const std::string& name)const{
    if(name == "H"){
        return H;
    }
    else if(name == "M"){
        return M;
    }

    throw std::logic_error("Invalid matrix name. Name "+name+ " not found");
}

const DynVec<real_t> get_measurement(const SysState<3>& state){
   return DynVec<real_t>({state.get("X"), state.get("Y")});
}


}

int main() {
   
    using namespace example;
    uint_t n_steps = 300;

    auto time = 0.0;
    auto dt = 0.5;

    /// angular velocity
    auto w = 0.0;

    /// linear velocity
    auto vt = 1.0;

    std::array<real_t, 2> motion_control_error;
    motion_control_error[0] = 0.0;
    motion_control_error[1] = 0.0;

    DiffDriveDynamics exact_motion_model;
    exact_motion_model.set_matrix_update_flag(false);
    exact_motion_model.set_time_step(dt);

    DiffDriveDynamics motion_model;

    motion_model.initialize_matrices({1.0, 0.0, motion_control_error});
    motion_model.set_time_step(dt);

    ObservationModel observation;

    ExtendedKalmanFilter<DiffDriveDynamics, ObservationModel> ekf(motion_model, observation);

    DynMat<real_t> R(2, 2, 0.0);
    R(0,0) = 1.0;
    R(1, 1) = 1.0;

    DynMat<real_t> Q(2, 2, 0.0);
    Q(0,0) = 0.001;
    Q(1, 1) = 0.001;

    DynMat<real_t> P(3, 3, 0.0);
    P(0, 0) = 1.0;
    P(1, 1) = 1.0;
    P(2, 2) = 1.0;

    ekf.set_matrix("P", P);
    ekf.set_matrix("R", R);
    ekf.set_matrix("Q", Q);

    kernel::CSVWriter writer("state", kernel::CSVWriter::default_delimiter(), true);
    std::vector<std::string> names{"X", "Y", "X_true", "Y_true"};
    writer.write_column_names(names);

    try{

        uint_t counter=0;
        for(uint_t step=0; step < n_steps; ++step){

            std::cout<<"At step: "<<step<<" time: "<<time<<std::endl;

            if(counter == 50){
              w = kernel::UnitConverter::degrees_to_rad(45.0);
            }
            else if(counter == 100){
               w = kernel::UnitConverter::degrees_to_rad(-45.0);
            }
            else if(counter == 150){
               w = kernel::UnitConverter::degrees_to_rad(-45.0);
            }
            else{
                w = 0.0;
            }


            auto motion_input = std::make_tuple(vt, w, motion_control_error);
            auto& exact_state = exact_motion_model.evaluate(motion_input);

            ekf.predict(motion_input);

            auto& state = motion_model.get_state();
            auto z = get_measurement(state);
            ekf.update(z);

            std::cout<<"Position: "<<ekf.get("X")<<", "<<ekf.get("Y")<<std::endl;
            std::cout<<"Orientation: "<<kernel::UnitConverter::rad_to_degrees(ekf.get("Theta"))<<std::endl;
            std::cout<<"V: "<<vt<<", W: "<<w<<std::endl;

            std::vector<real_t> row(4, 0.0);
            row[0] = exact_state.get("X");
            row[1] = exact_state.get("Y");
            row[2] = state.get("X");
            row[3] = state.get("Y");
            writer.write_row(row);

            time += dt;
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

