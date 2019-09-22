#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"

#include <random>
#include <iostream>

namespace exe
{
using kernel::real_t;
using kernel::uint_t;

// number of iterations
const uint_t N_ITERATIONS = 100000;

real_t f(real_t x){
    return x*x;
}

}

int main(){

    using namespace exe;

    // lower and upper limits
    real_t a = 1.0;
    real_t b = 3.0;

    real_t total_area = 0.0;
    real_t area_under_curve = 0.0;

    std::default_random_engine x_generator;
    std::uniform_real_distribution<real_t> x_distribution(a, b);

    auto y_lower = 0.0;
    auto y_upper = f(b);
    std::default_random_engine y_generator;
    std::uniform_real_distribution<real_t> y_distribution(y_lower, y_upper);

    // the area of the rectangle [a,b]x[f(a), f(b)]
    const real_t RECT_AREA = (b-a)*(f(b)-0.0);


    for(uint_t itr=0; itr < N_ITERATIONS; ++itr){

        // generate random x and y points
        real_t x = x_distribution(x_generator);
        real_t y = y_distribution(y_generator);

        std::cout<<"Generated point: "<<x<<","<<y<<std::endl;

        // add 1 to count of points within the whole area
        total_area += 1.0;

        if( y  <= f(x)){
           area_under_curve += 1;
        }
    }

    std::cout<<"Rectangle area: "<<RECT_AREA<<std::endl;
    if(area_under_curve != 0.){

        std::cout<<"Total area points: "<<area_under_curve<<std::endl;
        std::cout<<"Area under curve points: "<<area_under_curve<<std::endl;
        std::cout<<"Calculated area: "<<RECT_AREA*(area_under_curve/total_area)<<std::endl;
    }

    return 0;
}



