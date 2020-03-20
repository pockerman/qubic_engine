#include "kernel/base/types.h"

#include <iostream>

namespace example
{
using kernel::real_t;
using kernel::uint_t;
}

int main(){

    using namespace example;

    try{


    }
    catch(std::logic_error& error){

        std::cerr<<error.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}





