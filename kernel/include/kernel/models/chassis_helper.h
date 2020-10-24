#ifndef CHASSIS_HELPER_H
#define CHASSIS_HELPER_H


#include "kernel/base/types.h"

#include <boost/any.hpp>
#include <map>
#include <string>

namespace kernel {
namespace models {

///
/// \brief The ChassisHelpers class. Wraps common functionality
/// for chassis models
///
class ChassisHelper
{
public:

    ///
    /// \brief BicycleVehicleChassis. Constructor
    ///
    ChassisHelper();

    ///
    /// \brief ~BicycleVehicleChassis. Destructor
    ///
    ~ChassisHelper();

    ///
    /// \brief get_metric_system. Return the metric system
    ///
    const std::string& get_metric_system()const{return metric_system_;}

    ///
    /// \brief set_metric_system. Set the metric system
    ///
    void set_metric_system(const std::string& msys){metric_system_ = msys;}

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
    /// \brief set_description_file. Set the file describing the chassis
    ///
    void set_description_file(const std::string& name){file_description_ = name;}


protected:

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

};

}

}

#endif // CHASSIS_BASE_H
