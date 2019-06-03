/* 
 * File:   exe.h
 * Author: david
 *
 * Created on October 25, 2015, 10:26 AM
 */

#ifndef EXE_H
#define	EXE_H

#include "cubic_engine/base/cubic_engine_types.h"


namespace exe2{

using cengine::DynVec;
using cengine::real_t;
using cengine::uint_t;

// The class that describes the dynamics of the robot
// The following model is useed
// $$ \dot{x} = vcos(\phi)$$
// $$ \dot{y} = vsin((\phi)$$
// $$ \dot{\phi} = \omega$$

class Dynamics
{

public:

    template<typename... Args>
    void execute(const Args&... args);

};

// The robot to simulate
class Robot
{
public:


    void simulate();

private:

    // the state of the robot:
    // x, y are a 2D x-y position, $\phi$ is orientation, and v is velocity
    DynVec<real_t> state_;

    // The object that handles the dynamics of the robot
    Dynamics dynamics;
};

}//exe2



#endif	/* EXE_H */

