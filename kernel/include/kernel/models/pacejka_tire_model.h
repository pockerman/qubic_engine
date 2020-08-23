#ifndef PACEJKA_TIRE_MODEL_H
#define PACEJKA_TIRE_MODEL_H

#include "kernel/base/types.h"
#include "kernel/models/tire_model_base.h"

#include <map>
namespace kernel{
namespace models {

class PacejkaTireModel:public TireModelBase
{
public:

    ///
    /// \brief force_t The type of the force
    ///
    typedef TireModelBase::force_t force_t;

    ///
    /// \brief PacejkaTireModel. Constructor
    ///
    PacejkaTireModel();

    ///
    /// \brief compute_forces. Compute the forces
    /// exerted on the tire
    ///
    virtual force_t compute_force()const override final;

    ///
    /// \brief load_from_json Load the model from a JSON description
    ///
    virtual void load_from_json(const std::string& filename) override final;

private:

    ///
    /// \brief properties_ Scalar values used by the model to compute
    /// the forces
    ///
    std::map<std::string, real_t> properties_;
};

}
}

#endif // PACEJKA_TIRE_MODEL_H
