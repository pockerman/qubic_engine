#include "kernel/models/tire_model_factory.h"
#include "kernel/models/pacejka_tire_model.h"

#include "nlohmann/json.hpp"

#include <stdexcept>
#include <fstream>

namespace kernel {
namespace models {

std::unique_ptr<TireModelBase>
TireModelFactory::build(TireModelType type){

    std::unique_ptr<TireModelBase> ptr;
    switch (type)
    {
    case TireModelType::PACEJKA:
        ptr = std::make_unique<PacejkaTireModel>();
    default:

        throw std::logic_error("Inavlid TireModelType");
    }

    return ptr;
}

std::unique_ptr<TireModelBase>
TireModelFactory::build_from_json_file(const std::string& filename){

    using json = nlohmann::json;
    json json_data;
    std::ifstream file_stream(filename);

    // read from the file stream
    file_stream >> json_data;

    std::string type_str = static_cast<std::string>(json_data["type"]);

    auto ptr = TireModelFactory::build(TireModelHelpers::sring_to_type(type_str));
    ptr->load_from_json(filename);

    return ptr;
}

}

}
