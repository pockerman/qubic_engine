#ifndef TIRE_MODEL_BASE_H
#define TIRE_MODEL_BASE_H

#include "kernel/base/types.h"
#include "kernel/models/tire_model_type.h"

namespace kernel {
namespace models {


class TireModelBase
{
public:

    ///
    /// \brief force_t The type of the force
    ///
    typedef DynVec<real_t> force_t;

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
    /// \brief compute_forces. Compute the forces
    /// exerted on the tire
    ///
    virtual force_t compute_force()const=0;

    ///
    /// \brief load_from_json Load the model from a JSON description
    ///
    virtual void load_from_json(const std::string& filename)=0;


protected:

    ///
    /// \brief TireModelBase Constructor
    ///
    TireModelBase(TireModelType type);


    ///
    /// \brief type_ Type of the model
    ///
    TireModelType type_;

};

inline
TireModelBase::TireModelBase(TireModelType type)
    :
      type_(type)
{}

}
}

#endif // TIRE_MODEL_BASE_H
