#ifndef QUADROTOR_CHASSIS_H
#define QUADROTOR_CHASSIS_H

#include "kernel/base/types.h"
#include "kernel/models/chassis_helper.h"

#include <boost/any.hpp>
#include <map>
#include <string>

namespace kernel {
namespace models {

class QuadrotorChassis
{
public:

    ///
    /// \brief moment_t The type describing the inertia moments
    ///
    typedef DynMat<real_t> inertia_moment_t;

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
    QuadrotorChassis();

    ///
    /// \brief ~BicycleVehicleChassis. Destructor
    ///
    ~QuadrotorChassis();

    ///
    /// \brief load_from_json Load the description of the chassis from
    /// the given json file
    ///
    void load_from_json(const std::string& filename);

    ///
    /// \brief get_scalar_property Returns the scalar property
    /// with the given name
    ///
    real_t get_scalar_property(const std::string& name)const{return helper_.get_scalar_property(name);}

    ///
    /// \brief set_scalar_property set the scalar propery
    /// with the given name
    ///
    void set_scalar_property(const std::string& name, real_t value){helper_.set_scalar_property(name, value);}

    ///
    /// \brief get_description_file Returns the file describing this chassis
    ///
    const std::string_view get_description_file()const{return helper_.get_description_file();}

    ///
    /// \brief compute_forces Compute the forces
    /// acting on the chassis
    ///
    force_t compute_force(const force_input_t& input)const;

    ///
    /// \brief n_engines. Returns the number of engines
    ///
    uint_t n_engines()const{return 4;}

private:

    ///
    /// \brief helper_.
    ///
    ChassisHelper helper_;
};

}

}

#endif // QUADROTOR_CHASSIS_H
