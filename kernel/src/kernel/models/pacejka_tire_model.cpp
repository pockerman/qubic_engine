#include "kernel/models/pacejka_tire_model.h"
#include "kernel/models/tire_model_type.h"

#include "nlohmann/json.hpp" // reading from json file

namespace kernel{
namespace models {

PacejkaTireModel::PacejkaTireModel()
    :
    TireModelBase(TireModelType::PACEJKA),
    properties_()
{}


PacejkaTireModel::force_t
PacejkaTireModel::compute_force()const{

}

void
PacejkaTireModel::load_from_json(const std::string& filename){

}

}
}
