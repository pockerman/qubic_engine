#include "cubic_engine/base/cubic_engine_types.h"

#include <cmath>
#include <iostream>
#include <tuple>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::DynVec;

}

int main() {
   
    using namespace example;
    
    DynMat<real_t> X(6, 2);
    X(0,0) = -1.;
    X(0,1) = -1.;
    
    X(1,0) = -2.;
    X(1,1) = -1.;
    
    X(2,0) = -3.;
    X(2,1) = -2.;
    
    X(3,0) = 1.;
    X(3,1) = 1.;
    
    X(4,0) = 2.;
    X(4,1) = 1.;
    
    X(5,0) = 3.;
    X(5,1) = 2.;

    
    try{

        
        }
    }
    catch(std::runtime_error& e){
        std::cerr<<"Runtime error: "
                 <<e.what()<<std::endl;
    }
    catch(std::logic_error& e){
        std::cerr<<"Logic error: "
                 <<e.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }
   
    return 0;
}

