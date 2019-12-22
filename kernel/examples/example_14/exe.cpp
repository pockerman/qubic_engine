#include "kernel/base/config.h"

#ifdef USE_OPENMP

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/parallel/threading/task_base.h"
#include "kernel/parallel/threading/openmp_executor.h"
#include "kernel/parallel/utilities/reduction_operations.h"

#include <random>
#include <iostream>

namespace exe
{
using kernel::real_t;
using kernel::uint_t;

// number of iterations
const uint_t N_ITERATIONS = 100000;

// lower and upper limits
const real_t a = 1.0;
const real_t b = 3.0;

real_t f(real_t x){
    return x*x;
}

class Task: public kernel::TaskBase
{
public:

     Task()
         :
       TaskBase(),
       a_(a),
       b_(b),
       result_(0.0)
     {}

     real_t get_result()const{return result_;}

protected:

     virtual void run() override final;

private:

     const real_t a_;
     const real_t b_;
     real_t result_;
};

void
Task::run(){

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());

    std::uniform_real_distribution<real_t> x_distribution(a_, b_);

    auto y_lower = 0.0;
    auto y_upper = f(b_);
    std::uniform_real_distribution<real_t> y_distribution(y_lower, y_upper);

    // generate random x and y points
    real_t x = x_distribution(gen);
    real_t y = y_distribution(gen);

    if( y  <= f(x)){
       result_ += 1;
    }
}

}

int main(){

    using namespace exe;

    // the area of the rectangle [a,b]x[f(a), f(b)]
    const real_t RECT_AREA = (b-a)*(f(b)-0.0);

    kernel::OMPExecutor executor(4);
    kernel::Sum<real_t> operation;
    executor.parallel_for_reduce<Task, kernel::Sum<real_t>>(N_ITERATIONS, operation);

    if(!operation.is_result_valid()){

        std::cout<<"Result is not valid..."<<std::endl;
    }
    else{

        real_t total_area = static_cast<real_t>(N_ITERATIONS);
        real_t area_under_curve = operation.get_resource();
        std::cout<<"Rectangle area: "<<RECT_AREA<<std::endl;

        if(area_under_curve != 0.){
            std::cout<<"Total area points: "<<total_area<<std::endl;
            std::cout<<"Area under curve points: "<<area_under_curve<<std::endl;
            std::cout<<"Calculated area: "<<RECT_AREA*(area_under_curve/total_area)<<std::endl;
        }
    }

    return 0;
}
#else
#include <iostream>
int main(){
    std::cout<<"You need OpenMP to run this example. Reconfigure kernel with OpenMP support"<<std::endl;
}
#endif



