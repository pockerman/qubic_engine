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
      helper_(),
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

    helper_.set_metric_system(static_cast<std::string>(json_data["metric_system"]));
    helper_.set_scalar_property("mass", static_cast<real_t>(json_data["mass"]));
    helper_.set_scalar_property("inertia", static_cast<real_t>(json_data["inertia_moment"]));
    helper_.set_scalar_property("l_R", static_cast<real_t>(json_data["l_R"]));
    helper_.set_scalar_property("l_F", static_cast<real_t>(json_data["l_F"]));

    // load the tire model
    std::string front_tire_model_file = static_cast<std::string>(json_data["front_tire_model_filename"]);
    std::string rear_tire_model_file = static_cast<std::string>(json_data["rear_tire_model_filename"]);

    front_tire_ = TireModelFactory::build_from_json_file(front_tire_model_file);
    rear_tire_ = TireModelFactory::build_from_json_file(rear_tire_model_file);

    // finally set the file
    helper_.set_description_file(filename);

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
