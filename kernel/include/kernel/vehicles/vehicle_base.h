#ifndef VEHICLE_BASE_H
#define VEHICLE_BASE_H

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
    ChronoVehicleWrapper();

private:

    chrono::vehicle::ChVehicle* vehicle_;

};
}

#endif // VEHICLE_BASE_H
