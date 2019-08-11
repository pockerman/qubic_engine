#ifndef EXE_H
#define EXE_H

#include "kernel/base/types.h"
#include "chrono_models/vehicle/sedan/Sedan.h"
#include "chrono_vehicle/wheeled_vehicle/utils/ChWheeledVehicleIrrApp.h"
#include "chrono_vehicle/terrain/RigidTerrain.h"

#include <string>
#include <memory>

namespace exe
{

using kernel::real_t;
using namespace chrono::vehicle::sedan;

// carries the used simulation data
struct SimData
{
    std::string BASIC_DATA_PATH; //("/home/david/MyProjects/cubic_engine/chrono_lib/chrono/data/vehicle/paths/");

    // Rigid terrain dimensions
    real_t TERRAIN_HEIGHT; //= 0;
    real_t TERRAIN_LENGTH; //= 300.0;  // size in X direction
    real_t TERRAIN_WIDTH; // = 300.0;   // size in Y direction
    std::string TERRAIN_PATH; //("/home/david/MyProjects/cubic_engine/chrono_lib/chrono/data/vehicle/terrain/textures/tile4.jpg");

    // Input file names for the path-follower driver model
    const std::string PATH_FILE; //(BASIC_DATA_PATH + "straight.txt");

    // Initial vehicle location and orientation
    //const ChVector<> INIT_LOC(-125, -125, 0.5);
    //const ChQuaternion<> INIT_ROT(1, 0, 0, 0);

    // Desired vehicle speed (m/s)
    real_t target_speed = 20;

    // Point on chassis tracked by the chase camera
    chrono::ChVector<> TRACK_POINT; //(0.0, 0.0, 1.75);

    // Simulation step size
    const real_t DELTA_T = 2e-3;
    const real_t TIRE_DELTA_T = 1e-3;

    // Simulation end time
    const real_t TIME_END = 100;

    // Render FPS
    const real_t FPS = 60;

    // Debug logging
    const bool DEBUG_OUTPUT = false;
    const real_t DEBUG_FPS = 10;

    // Output directories
    const std::string OUT_DIR; // = GetChronoOutputPath() + "STEERING_CONTROLLER";
    const std::string POV_DIR; // = OUT_DIR + "/POVRAY";

    // POV-Ray output
    const bool POVRAY_OUTPUT = false;

    // Vehicle state output (forced to true if povray output enabled)
    bool state_output = false;
    const int FILTER_WINDOW_SIZE = 20;
};

// forward declaration
class Vehicle;

// The class that will handle the simulation
// window
class Simulator
{

public:

    // constructor
    Simulator(const SimData& data);

    // initialize the simulator
    void initialize();

    // simulate
    void simulate();

private:

    // the used simulation data
    SimData sim_data_;

    // the vehicle that is simulated
    std::shared_ptr<Vehicle> vehicle_;

    // the application
    std::unique_ptr<chrono::vehicle::ChVehicleIrrApp> app_;

    // the terrain the vehicle is moving
    std::unique_ptr<chrono::vehicle::RigidTerrain> terrain_;

    void initialize_app();

    void initialize_terrain();
};

/**
  * Brief the class tha handles the vhicle
  */
class Vehicle
{

public:

    // The vehicle we want to simulate
    Sedan vehicle;

protected:



};


}

#endif // EXE_H
