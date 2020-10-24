/**
 * In this test we play the game in 
 */

#include "robosim/models/fsm_base.h"
#include "robosim/models/fsm_state_base.h"

#ifdef ROBOSIM_DEBUG
#include "utilities/exceptions.h"
#include <string>
#endif

#include <array>
#include <vector>
#include <utility>
#include <memory>

using namespace robosim;

namespace exe
{

class RobotMiner: public FSMBase
{

public:
        
      //constructor
      RobotMiner();
      
      
      //execute the FSM
      void execute();
      
private:
    
    //the higher the value, the thirstier the miner
    int thirst_level_;
        
        
};

RobotMiner::RobotMiner()
:
FSMBase(0,true,"RobotMiner"),
thirst_level_(0)
{}

void 
RobotMiner::execute(){
    
    thirst_level_ += 1;
    
    if(this->state_ptr_){
        
        state_ptr_->execute(*this);
    }
}


}//exe

int main(int argc, char** argv) {

    using namespace exe;

    try{

	std::cout<<"Starting simulation..."<<std::endl;
        RobotMiner miner;
        miner.execute();

		//run the simulation for 5 steps with dt = 0.01
		//SerialSimulator::init(5,0.01);

		//add the model to simulate
		//Robot* rptr = SerialSimulator::add_model<Robot>();

    }
    catch(utilities::ExceptInvalidIndex& e){
	std::cerr<<e.what()<<std::endl;
    }
    catch(...){
	std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }

    return 0;
}

