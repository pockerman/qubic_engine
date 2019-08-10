#ifndef VEHICLE_BASE_H
#define VEHICLE_BASE_H

#include <string>

/// forward declaration for chrono::vehicle::ChVehicle
namespace chrono
{
 namespace vehicle
 {
   class ChVehicle;
 }
}

namespace kernel
{

/**
 * @brief The ChronoVehicleWrapper class.
 * This is a wrapper to the Chrono::ChVehicle class
 */
class ChronoVehicleWrapper
{

public:

    /// \brief Constructor
    ChronoVehicleWrapper(const std::string& name);

private:

    chrono::vehicle::ChVehicle* vehicle_;

};
}

#endif // VEHICLE_BASE_H
