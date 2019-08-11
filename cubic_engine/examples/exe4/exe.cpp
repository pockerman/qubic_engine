#include "cubic_engine/integrators/fwd_euler.h"
#include "cubic_engine/control/pid_controller.h"
#include "kernel/base/physics_constants.h"
#include "kernel/utilities/csv_file_writer.h"

#include <iostream>
#include <cmath>
namespace exe
{  

using cengine::real_t;
using cengine::FWDEulerInt;

class VehicleDynamics
{

public:

    // constructor
    VehicleDynamics(real_t dt);

    // execute the integration
    void integrate(const real_t* u);

    real_t get_state()const{return velocity_;}
private:

    class ComputeRhs
    {

    public:

        ComputeRhs()=default;

        real_t operator()(real_t dt, const std::vector<real_t>& old_solution, const real_t* input);
    };

    real_t velocity_;
    cengine::FWDEulerInt<real_t, ComputeRhs> velocity_int_;
};

real_t
VehicleDynamics::ComputeRhs::operator()(real_t dt, const std::vector<real_t>& old_solution, const real_t* input){

    auto a = input[0];
    auto b = input[1];
    auto mass = 1000.0;
    auto yn = old_solution[0];
    auto u = input[2];
    auto d = input[3];

    return (-a/mass)*yn + (b/mass)*u - d;
}

VehicleDynamics::VehicleDynamics(real_t dt)
    :
      velocity_(0.0),
      velocity_int_(velocity_, dt)
{}

void
VehicleDynamics::integrate(const real_t* u){
    velocity_int_.integrate(u);
}
    
}

int main() {

    using cengine::uint_t;
    using cengine::real_t;

    auto Ki = 0.5;
    auto Kp = 1.5;
    auto Kd = 0.0;

    // number of simulation steps
    uint_t n_steps = 30000;

    // the time step
    auto dt = 0.001;

    // reference velocity in m/s
    auto r = 25.0;

    // road slope
    auto alpha = 5.0;

    // Ns/m
    auto a = 600.0;

    // kN/rad
    auto b = 10.0;

    real_t input[] = {a, b, 0.0, 0.0};

    // the world time
    auto time = 0.0;

    // the controller to use
    cengine::PIDControl controller(Kp, Kd, Ki);
    exe::VehicleDynamics dynamics(dt);

    kernel::CSVWriter writer("velocity", kernel::CSVWriter::default_delimiter(), true);
    std::vector<std::string> names{"t", "V", "Vref"};
    writer.write_column_names(names);

    std::vector<real_t> row(3);

    //time loop
    for(uint_t step=0; step < n_steps; ++step){

        std::cout<<"At time: "<<time<<std::endl;

        const auto y = dynamics.get_state();

        std::cout<<"\tVelocity is "<<y<<std::endl;

        const auto error = r - y;
        const auto u = controller.execute(error);

        input[2] = u;

        dynamics.integrate(input);

        // at 1 second change reference signal to 30 m/s
        if(step == 2000){
            r = 30.0;
        }

        // at t=10 there is a slope at the road
        if( step == 10000){
            input[3] = kernel::PhysicsConsts::gravity_constant()*std::sin(alpha);
        }

        time += dt;

        row[0] = time;
        row[1] = y;
        row[2] = r;

        writer.write_row(row);
    }
    
    return 0;
}

