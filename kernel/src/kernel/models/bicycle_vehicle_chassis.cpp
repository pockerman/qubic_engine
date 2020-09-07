#include "kernel/base/config.h"
#include "kernel/models/bicycle_vehicle_chassis.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/base/physics_constants.h"
#include "kernel/models/tire_model_factory.h"
#include "kernel/models/tire_model_base.h"

#include "nlohmann/json.hpp"

#include <fstream>
#include <stdexcept>

namespace kernel {
namespace models {

BicycleVehicleChassis::BicycleVehicleChassis()
    :
      file_description_(KernelConsts::dummy_string()),
      metric_system_(PhysicsConsts::default_metric_system()),
      scalar_props_(),
      front_tire_(),
      rear_tire_()
{}

BicycleVehicleChassis::~BicycleVehicleChassis()
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

    // load the tire model
    std::string front_tire_model_file = static_cast<std::string>(json_data["front_tire_model_filename"]);
    std::string rear_tire_model_file = static_cast<std::string>(json_data["rear_tire_model_filename"]);

    front_tire_ = TireModelFactory::build_from_json_file(front_tire_model_file);
    rear_tire_ = TireModelFactory::build_from_json_file(rear_tire_model_file);

    // finally set the file
    file_description_ = filename;

}

real_t
BicycleVehicleChassis::get_scalar_property(const std::string& name)const{

    auto itr = scalar_props_.find(name);

    if(itr == scalar_props_.end()){
        throw std::logic_error("Scalar property does not exist: " + name);
    }

    return itr->second;
}

void
BicycleVehicleChassis::set_scalar_property(const std::string& name, real_t value){
    scalar_props_.insert_or_assign(name, value);
}

BicycleVehicleChassis::force_t
BicycleVehicleChassis::compute_force(const BicycleVehicleChassis::force_input_t& input)const{

    if(!front_tire_){
        throw std::logic_error("Null front tire model");
    }

    if(!rear_tire_){
        throw std::logic_error("Null rear tire model");
    }

    auto l_R = get_scalar_property("l_R");
    auto l_F = get_scalar_property("l_F");

    typedef TireModelBase::input_t tire_model_input_t;

    auto delta = boost::any_cast<real_t>(get_property_from_input(input, "delta"));
    auto vx = boost::any_cast<real_t>(get_property_from_input(input, "vx"));
    auto vy = boost::any_cast<real_t>(get_property_from_input(input, "vy"));
    auto r = boost::any_cast<real_t>(get_property_from_input(input, "r"));

    auto alpha_r = std::atan((vy - l_R*r)/vx);
    auto alpha_f = std::atan((vy + l_F*r)/vx) - delta;

    tire_model_input_t tire_rear_in;
    tire_rear_in.insert_or_assign("apha", alpha_r);
    auto Fr = rear_tire_->compute_force(tire_rear_in);

    tire_model_input_t tire_front_in;
    tire_front_in.insert_or_assign("apha", alpha_f);
    auto Ff = front_tire_->compute_force(tire_front_in);



}

const boost::any&
BicycleVehicleChassis::get_property_from_input(const BicycleVehicleChassis::force_input_t& input, const std::string& name)const{

    auto itr = input.find(name);

    if(itr == input.end()){
        throw std::logic_error("Input  does not exist" + name);
    }

    return itr->second;
}

}

}
