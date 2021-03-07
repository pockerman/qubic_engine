#ifndef TIRE_MODEL_TYPE_H
#define TIRE_MODEL_TYPE_H

#include <string>

namespace kernel {
namespace models {

enum class TireModelType{PACEJKA, INVALID_TYPE};

///
/// \brief type_to_string convert TireModelType to std::string
///
std::string type_to_string(TireModelType type);

struct TireModelHelpers
{
    static TireModelType sring_to_type(const std::string& type);

};

}

}

#endif // TIRE_MODEL_TYPE_H
