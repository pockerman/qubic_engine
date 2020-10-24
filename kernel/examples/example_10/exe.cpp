#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/geometry/shapes/circle.h"
#include "kernel/geometry/geom_point.h"

#include <random>
#include <iostream>
#include <array>


int main(){

    using kernel::real_t;
    using kernel::uint_t;
    using kernel::GeomPoint;

    // number of iterations
    const uint_t N_ITERATIONS = 100000;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());

    kernel::Circle circle(2.0);

    real_t x0 = - circle.radius();
    real_t x1 = circle.radius();

    // distribution of the x-coordinate
    std::uniform_real_distribution<real_t> x_distribution(x0, x1);

    auto y0 = - circle.radius();
    auto y1 = circle.radius();
    std::uniform_real_distribution<real_t> y_distribution(y0, y1);

    real_t total_area = 0.0;
    real_t area_under_curve = 0.0;

    // the area of the rectangle
    const real_t RECT_AREA = (x1-x0)*(y1-y0);

    // the coordinates of the generated point
    std::array<real_t, 2> coords;

    for(uint_t itr=0; itr < N_ITERATIONS; ++itr){

        // generate random x and y points
        coords[0] = x_distribution(gen);
        coords[1] = y_distribution(gen);

        GeomPoint<2> point(coords);

        std::cout<<"Generated point: "<<coords[0]<<","<<coords[0]<<std::endl;

        // add 1 to count of points within the whole area
        total_area += 1.0;

        if( circle.is_inside(point)){
           area_under_curve += 1;
        }
    }

    std::cout<<"Rectangle area: "<<RECT_AREA<<std::endl;
    std::cout<<"Total area points: "<<total_area<<std::endl;

    if(area_under_curve != 0.){

        std::cout<<"Area under curve points: "<<area_under_curve<<std::endl;
        std::cout<<"Calculated area: "<<RECT_AREA*(area_under_curve/total_area)<<std::endl;
        std::cout<<"Circle area: "<<circle.area()<<std::endl;
    }

    return 0;
}



