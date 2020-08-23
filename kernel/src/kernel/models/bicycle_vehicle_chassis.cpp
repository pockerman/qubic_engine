#include "kernel/base/config.h"
#include "kernel/models/bicycle_vehicle_chassis.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/base/physics_constants.h"

#include "nlohmann/json.hpp"


#include <fstream>

namespace kernel {
namespace models {

BicycleVehicleChassis::BicycleVehicleChassis()
    :
      file_description_(KernelConsts::dummy_string()),
      metric_system_(PhysicsConsts::default_metric_system())
{}


void
BicycleVehicleChassis::load_from_json(const std::string& filename){

    using json = nlohmann::json;
    json json_data;
    std::ifstream file_stream(filename);

    // read from the file stream
    file_stream >> json_data;

    metric_system_ = static_cast<std::string>(json_data["metric_system"]);
    scalar_props_.insert_or_assign("mass", static_cast<real_t>(json_data["mass"]));
    scalar_props_.insert_or_assign("inertia", static_cast<real_t>(json_data["inertia_moment"]));
    scalar_props_.insert_or_assign("l_R", static_cast<real_t>(json_data["l_R"]));
    scalar_props_.insert_or_assign("l_F", static_cast<real_t>(json_data["l_F"]));


    // finally set the file
    file_description_ = filename;

}


void
BicycleVehicleChassis::set_scalar_property(const std::string& name, real_t value){
    scalar_props_.insert_or_assign(name, value);
}

}

}
