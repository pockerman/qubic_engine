#include "kernel/models/pacejka_tire_model.h"
#include "kernel/models/tire_model_type.h"

#include "nlohmann/json.hpp" // reading from json file

#include <fstream>
#include <cmath>
#include <stdexcept>

namespace kernel{
namespace models {

PacejkaTireModel::PacejkaTireModel()
    :
    TireModelBase(TireModelType::PACEJKA, 1, 1),
    properties_()
{}

real_t
PacejkaTireModel::get_scalar_property(const std::string& name)const{

    auto itr = properties_.find(name);

    if(itr == properties_.end()){
        throw std::logic_error("Invalid property name: " + name);
    }

    return itr->second;
}

PacejkaTireModel::force_t
PacejkaTireModel::compute_force(const PacejkaTireModel::input_t& input)const{

    PacejkaTireModel::force_t f(this->output_dim(), 0.0);
    auto itr = input.find("alpha");

    if(itr == input.end()){
        throw std::logic_error("alpha property not found in input");
    }

    auto alpha = boost::any_cast<real_t>(itr->second);

    auto Da = get_scalar_property("Da");
    auto Ca = get_scalar_property("Ca");
    auto Ba = get_scalar_property("Ba");

    f[0] = Da*std::sin(Ca*std::atan(Ba*alpha));
    return f;
}

void
PacejkaTireModel::load_from_json(const std::string& filename){

    using json = nlohmann::json;
    json json_data;
    std::ifstream file_stream(filename);

    // read from the file stream
    file_stream >> json_data;

    properties_.insert_or_assign("Ba", static_cast<real_t>(json_data["Ba"]));
    properties_.insert_or_assign("Ca", static_cast<real_t>(json_data["Ca"]));
    properties_.insert_or_assign("Da", static_cast<real_t>(json_data["Da"]));

}

}
}
