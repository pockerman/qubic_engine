#include "cubic_engine/integrators/fwd_euler.h"
#include "cubic_engine/control/pid_controller.h"

#include <iostream>
namespace exe
{  
    
}


int main(int argc, char** argv) {

    using cengine::uint_t;

    auto Ki = 1.0;
    auto Kp = 0.5;
    auto Kd = 0.0;

    // the controller to use
    cengine::PIDControl controller(Kp, Kd, Ki);

    // number of simulation steps
    auto n_steps = 30000;

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

    // the world time
    auto time = 0.0;

    //time loop
    for(uint_t step=0; step < n_steps; step){

        std::cout<<"At time: "<<time<<std::endl;

        y = vehicle.state

        print("\tVelocity is %f "%y)

        error = r - y

        u = controller.execute(error)

        kwargs['u'] = u
        vehicle.execute(**kwargs)

        if step == 1000: # at 1 second change reference signal to 30 m/s
            r = 30.0

        if step == 10000: # at t=10 there is a slope at the road
            kwargs['d'] = Physics.gravity_constant()*math.sin(alpha)

        time += dt

        tarray.append(time)
        yarray.append(vehicle.state)

        t_ref.append(time)
        y_ref.append(r)
    }
    
    return 0;
}

