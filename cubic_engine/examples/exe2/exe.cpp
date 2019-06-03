/***
 * Extended Kalman Filter
 *
 *
 *
*/


#include "exe.h"

#include <iostream>

namespace exe2
{

template<typename... Args>
void
Dynamics::execute(const Args&... args){


}

void
Robot::simulate(){

}

}

int main(int argc, char** argv) {
   
    using namespace exe2;
    uint_t n_steps = 1000;
    real_t dt = 0.1;
    try{
        
        for(uint_t step=1; step < n_steps; ++step){

            std::cout<<"\tAt step: "<<step<<std::endl;

        }

    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }
   
  
    return 0;
}

