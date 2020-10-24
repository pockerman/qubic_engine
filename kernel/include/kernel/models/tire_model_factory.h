#ifndef TIRE_MODEL_FACTORY_H
#define TIRE_MODEL_FACTORY_H

#include "kernel/models/tire_model_type.h"

#include <memory>
#include <string>

namespace kernel {
namespace models {

class TireModelBase;

class TireModelFactory
{
public:

    ///
    /// \brief build Build a tire model
    ///
    static std::unique_ptr<TireModelBase> build(TireModelType type);

    ///
    /// \brief build Build a tire model from filename
    ///
    static std::unique_ptr<TireModelBase> build_from_json_file(const std::string& filename);

};

}

}

#endif // TIRE_MODEL_FACTORY_H
