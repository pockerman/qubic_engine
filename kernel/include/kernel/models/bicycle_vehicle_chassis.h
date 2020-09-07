#ifndef BICYCLE_VEHICLE_CHASSIS_H
#define BICYCLE_VEHICLE_CHASSIS_H

#include "kernel/base/types.h"

#include "boost/any.hpp"

#include <string>
#include <map>
#include <memory>

namespace kernel {
namespace models {


class TireModelBase;

class BicycleVehicleChassis
{
public:

    ///
    /// \brief moment_t The type describing the inertia moments
    ///
    typedef real_t inertia_moment_t;

    ///
    /// \brief force_t The type of the force
    ///
    typedef DynVec<real_t> force_t;

    ///
    /// \brief force_input_t Type of input for force calculation
    ///
    typedef std::map<std::string, boost::any> force_input_t;

    ///
    /// \brief BicycleVehicleChassis. Constructor
    ///
    BicycleVehicleChassis();

    ///
    /// \brief ~BicycleVehicleChassis. Destructor
    ///
    ~BicycleVehicleChassis();

    ///
    /// \brief load_from_json Load the description of the chassis from
    /// the given json file
    ///
    void load_from_json(const std::string& filename);

    ///
    /// \brief get_scalar_property Returns the scalar property
    /// with the given name
    ///
    real_t get_scalar_property(const std::string& name)const;

    ///
    /// \brief set_scalar_property set the scalar propery
    /// with the given name
    ///
    void set_scalar_property(const std::string& name, real_t value);

    ///
    /// \brief get_description_file Returns the file describing this chassis
    ///
    const std::string_view get_description_file()const{return file_description_;}

    ///
    /// \brief compute_forces Compute the forces
    /// acting on the chassis
    ///
    force_t compute_force(const force_input_t& input)const;

private:

    ///
    /// \brief file_description_
    ///
    std::string file_description_;

    ///
    /// \brief units_ The units used by the chassis
    ///
    std::string metric_system_;

    ///
    /// \brief scalar_props_ Scalar properties map
    ///
    std::map<std::string, real_t> scalar_props_;

    ///
    /// \brief left_tire_ Pointer to the left tire model
    ///
    std::unique_ptr<TireModelBase> front_tire_;

    ///
    /// \brief left_tire_ Pointer to the right tire model
    ///
    std::unique_ptr<TireModelBase> rear_tire_;

    ///
    /// \brief get_property_from_input Returns the named property
    /// from the given input
    ///
    const boost::any& get_property_from_input(const force_input_t& input, const std::string& name)const;

};
}

}

#endif // BICYCLE_VEHICLE_CHASSIS_H
