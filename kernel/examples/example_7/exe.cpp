#include "exe.h"

#include "chrono/utils/ChFilters.h"
#include "chrono_vehicle/driver/ChIrrGuiDriver.h"
#include "chrono_vehicle/driver/ChPathFollowerDriver.h"
#include "chrono_vehicle/utils/ChVehiclePath.h"

namespace exe
{

Simulator::Simulator(const SimData& data)
    :
      sim_data_(data),
      vehicle_(),
      app_(),
      terrain_()
{}

void
Simulator::initialize_app(){

    app_ = std::make_unique<chrono::vehicle::ChVehicleIrrApp>(&vehicle_->vehicle.GetVehicle(), &vehicle_->vehicle.GetPowertrain(),
                                                              L"Sraight Line Vehicle Motion Demo",  irr::core::dimension2d<irr::u32>(800, 640));
    app_->SetHUDLocation(500, 20);
    app_->SetSkyBox();
    app_->AddTypicalLogo();
    app_->AddTypicalLights(irr::core::vector3df(-150.f, -150.f, 200.f), irr::core::vector3df(-150.f, 150.f, 200.f), 100, 100);
    app_->AddTypicalLights(irr::core::vector3df(150.f, -150.f, 200.f), irr::core::vector3df(150.0f, 150.f, 200.f), 100, 100);
    app_->EnableGrid(false);
    app_->SetChaseCamera(sim_data_.TRACK_POINT, 6.0, 0.5);
    app_->SetTimestep(sim_data_.DELTA_T);
}

void
Simulator::initialize_terrain(){

    using chrono::ChVector;
    using chrono::ChCoordsys;
    using chrono::ChColor;

    terrain_ = std::make_unique<chrono::vehicle::RigidTerrain>(vehicle_->vehicle.GetSystem());

    auto patch = terrain_->AddPatch(ChCoordsys<>(ChVector<>(0, 0, sim_data_.TERRAIN_HEIGHT - 5), sim_data_.QUNIT),
                                    ChVector<>(sim_data_.TERRAIN_LENGTH, sim_data_.TERRAIN_WIDTH, 10));

    patch->SetContactFrictionCoefficient(0.8f);
    patch->SetContactRestitutionCoefficient(0.01f);
    patch->SetContactMaterialProperties(2e7f, 0.3f);
    patch->SetColor(ChColor(1, 1, 1));


    std::string texture_file = vehicle::GetDataFile(sim_data_.TERRAIN_PATH);
    GetLog()<<" Texture file is located at "<<texture_file<<"\n";
    patch->SetTexture(sim_data_.TERRAIN_PATH, 200, 200 );

    terrain_->Initialize();
}

void
Simulator::initialize(){

    initialize_app();
    initialize_terrain();
}

void
Simulator::simulate(){

    using chrono::ChVector;
    using chrono::vehicle::ChIrrGuiDriver;
    using chrono::vehicle::ChPathFollowerDriverXT;
    using chrono::ChBezierCurve;

    chrono::utils::ChRunningAverage fwd_acc_GC_filter(sim_data_.FILTER_WINDOW_SIZE);
    chrono::utils::ChRunningAverage lat_acc_GC_filter(sim_data_.FILTER_WINDOW_SIZE);
    chrono::utils::ChRunningAverage fwd_acc_driver_filter(sim_data_.FILTER_WINDOW_SIZE);
    chrono::utils::ChRunningAverage lat_acc_driver_filter(sim_data_.FILTER_WINDOW_SIZE);

    // Create a GUI driver for interactive inputs
    ChIrrGuiDriver driver_gui(*(app_.get()));
    driver_gui.Initialize();

    // Create the Bezier path from data file
    GetLog()<<" Bezeir path is at: "<<PATH_FILE<<"\n";
    auto path = chrono::ChBezierCurve::read(sim_data_.PATH_FILE);
    GetLog()<<"Set up path file"<<"\n";

    ChPathFollowerDriverXT driver_follower( vehicle_->vehicle.GetVehicle(), path, "my_path",
                                            sim_data_.target_speed, vehicle_->vehicle.GetVehicle().GetMaxSteeringAngle());
    driver_follower.GetSteeringController().SetLookAheadDistance(5);
    driver_follower.GetSteeringController().SetGains(0.4, 1, 1, 1);
    driver_follower.GetSpeedController().SetGains(0.4, 0, 0);
    driver_follower.Initialize();

    while (app_->GetDevice()->run()) {

            // Extract system state
            real_t time = vehicle_->vehicle.GetSystem()->GetChTime();

            ChVector<> acc_CG =     vehicle_->vehicle.GetVehicle().GetChassisBody()->GetPos_dtdt();
            ChVector<> acc_driver = vehicle_->vehicle.GetVehicle().GetVehicleAcceleration(driver_pos);

            real_t fwd_acc_CG = fwd_acc_GC_filter.Add(acc_CG.x());
            real_t lat_acc_CG = lat_acc_GC_filter.Add(acc_CG.y());
            real_t fwd_acc_driver = fwd_acc_driver_filter.Add(acc_driver.x());
            real_t lat_acc_driver = lat_acc_driver_filter.Add(acc_driver.y());

            // End simulation
            if (time >= sim_data_.TIME_END)
                break;

            // Collect output data from modules (for inter-module communication)
            real_t throttle_input = selector.GetDriver()->GetThrottle();
            real_t steering_input = selector.GetDriver()->GetSteering();
            real_t braking_input  = selector.GetDriver()->GetBraking();

            // Update sentinel and target location markers for the path-follower controller.
            // Note that we do this whether or not we are currently using the path-follower driver.
            const ChVector<>& pS = driver_follower.GetSteeringController().GetSentinelLocation();
            ballS->setPosition(irr::core::vector3df(static_cast<irr::f32>(pS.x()),
                                                    static_cast<irr::f32>(pS.y()),
                                                    static_cast<irr::f32>(pS.z())));

            const ChVector<>& pT = driver_follower.GetSteeringController().GetTargetLocation();
            ballT->setPosition(irr::core::vector3df(static_cast<irr::f32>(pT.x()),
                                                    static_cast<irr::f32>(pT.y()),
                                                    static_cast<irr::f32>(pT.z())));

            app_->BeginScene(true, true, irr::video::SColor(255, 140, 161, 192));
            app_->DrawAll();

            // Output POV-Ray data
            if (sim_frame % render_steps == 0) {

                if (POVRAY_OUTPUT) {
                    char filename[100];
                    sprintf(filename, "%s/data_%03d.dat", POV_DIR.c_str(), render_frame + 1);
                    chrono::utils::WriteShapesPovray(vehicle_->vehicle.GetSystem(), filename);
                }

                if (state_output) {
                    csv << time << steering_input << throttle_input << braking_input;
                    csv << vehicle.GetVehicle().GetVehicleSpeed();
                    csv << acc_CG.x() << fwd_acc_CG << acc_CG.y() << lat_acc_CG;
                    csv << acc_driver.x() << fwd_acc_driver << acc_driver.y() << lat_acc_driver;
                    csv << std::endl;
                }

                render_frame++;
            }

            // Debug logging
            //if (debug_output && sim_frame % debug_steps == 0) {

                ChVector<> pos = vehicle_->vehicle.GetVehicle().GetVehiclePos();
                double speed = vehicle_->vehicle.GetVehicle().GetVehicleSpeed();

                GetLog() << "Driver Acceleration:  " << acc_driver.x() << "  " << acc_driver.y() << "  " << acc_driver.z()
                         << "\n";
                GetLog() << "CG Acceleration:      " << acc_CG.x() << "  " << acc_CG.y() << "  " << acc_CG.z() << "\n";
                GetLog() << "\n";
                GetLog()<<"Vehicle Position: "<<pos.x()<<" "<<pos.y()<<" "<<pos.z()<<"\n";
                GetLog() << "\n";
                GetLog()<<"Vehicle Speed: "<<speed<<"\n";
            //}

            // Update modules (process inputs from other modules)
            driver_follower.Synchronize(time);
            driver_gui.Synchronize(time);
            terrain.Synchronize(time);
            vehicle.Synchronize(time, steering_input, braking_input, throttle_input, terrain);
            std::string msg = selector.UsingGUI() ? "GUI driver" : "Follower driver";
            app.Synchronize(msg, steering_input, throttle_input, braking_input);

            // Advance simulation for one timestep for all modules
            double step = realtime_timer.SuggestSimulationStep(sim_data_.DELTA_T);

            driver_follower.Advance(step);
            driver_gui.Advance(step);
            terrain.Advance(step);
            vehicle.Advance(step);
            app_->Advance(step);

            // Increment simulation frame number
            sim_frame++;
            app_->EndScene();
        }
}

}

int main(){

    using namespace exe;
    SimData data;
    Simulator simulator(data);
    simulator.initialize();
    simulator.simulate();

    return 0;
}



