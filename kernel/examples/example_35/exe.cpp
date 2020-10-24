#include "kernel/base/config.h"
#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/models/bicycle_vehicle_chassis.h"
#include "kernel/models/rigid_body_model.h"
#include "kernel/dynamics/bicycle_vehicle_model_dynamics.h"


#include <cmath>
#include <iostream>
#include <string>

int main(){

    try{

        using kernel::real_t;
        using kernel::uint_t;
        using kernel::models::RigidBodyModel;
        using kernel::models::BicycleVehicleChassis;
        using kernel::dynamics::BicycleVehicleModelDynamics;

        typedef RigidBodyModel<BicycleVehicleChassis, BicycleVehicleModelDynamics> rigid_body_t;

        std::string chassis_filename_data(PROJECT_PATH);
        chassis_filename_data += "/examples/example_35/chassis_data.json";
        rigid_body_t rb;
        rb.load_chassis_json(chassis_filename_data);


    }
    catch(std::logic_error& error){

        std::cerr<<error.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}





