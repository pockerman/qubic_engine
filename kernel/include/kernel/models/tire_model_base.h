#ifndef TIRE_MODEL_BASE_H
#define TIRE_MODEL_BASE_H

#include "kernel/base/types.h"
#include "kernel/models/force_calculator_base.h"
#include "kernel/models/tire_model_type.h"

namespace kernel {
namespace models {


class TireModelBase: public ForceCalculatorBase
{
public:

    ///
    /// \brief force_t The type of the force
    ///
    typedef ForceCalculatorBase::force_t force_t;

    ///
    /// \brief input_t The type of the input
    ///
    typedef ForceCalculatorBase::input_t input_t;

    ///
    /// \brief ~TireModelBase. Destructor
    ///
    virtual ~TireModelBase()
    {}

    ///
    /// \brief get_type Returns the type of the model
    ///
    TireModelType get_type()const{return type_;}

    ///
    /// \brief load_from_json Load the model from a JSON description
    ///
    virtual void load_from_json(const std::string& filename)=0;


protected:

    ///
    /// \brief TireModelBase Constructor
    ///
    TireModelBase(TireModelType type, uint_t output_dim, uint_t input_dim);

    ///
    /// \brief type_ Type of the model
    ///
    TireModelType type_;

};

inline
TireModelBase::TireModelBase(TireModelType type, uint_t output_dim, uint_t input_dim)
    :
      ForceCalculatorBase(output_dim, input_dim),
      type_(type)
{}

}
}

#endif // TIRE_MODEL_BASE_H
