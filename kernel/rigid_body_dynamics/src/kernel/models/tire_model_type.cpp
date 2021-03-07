#include "kernel/models/tire_model_type.h"
#include <boost/algorithm/string.hpp>

namespace kernel {
namespace models {

std::string type_to_string(TireModelType type){

    switch(type){
    case TireModelType::PACEJKA:
        return "PACEJKA";
    }

    return "INVALID_TYPE";
}

TireModelType
TireModelHelpers::sring_to_type(const std::string& type){

    std::string new_type(type);
    boost::to_upper(new_type);

    if(new_type == "PACEJKA"){
        return TireModelType::PACEJKA;
    }

    return TireModelType::INVALID_TYPE;
}


}

}
