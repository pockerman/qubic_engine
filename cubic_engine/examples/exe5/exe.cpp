#include "exe.h"

#include <iostream>

namespace exe
{

 Robot::Robot(const SimData& data)
     :
    pf_(data.num_parts, data.num_resample_parts)
 {}


void observation(const DynVec<real_t>& u){}

}

int main(){

    using namespace exe;


    try{

        SimData data {500, 100, 500};
        Input input;
        Robot robot(data);

        kernel::CSVWriter writer("robot_state", kernel::CSVWriter::default_delimiter(), true);
        //std::vector<std::string> names{"X", "Y", "X_true", "Y_true", "Phi", "V", "Zx", "Zy" };
        //writer.write_column_names(names);

        for(uint_t step=0; step < N_STEPS; ++step){

             std::cout<<"At step: "<<step<<std::endl;
             observation(input.get_control()); // update y and ud
             robot.simulate(input);
             robot.save_state(writer);
         }
        }
        catch(std::runtime_error& e){
            std::cerr<<e.what()<<std::endl;
        }
        catch(...){
            std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
        }


    return 0;
}



