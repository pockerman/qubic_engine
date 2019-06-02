/* 
 * File:   exe.cpp
 * Author: david
 *
 * Created on October 10, 2015, 12:28 PM
 */

// In this example we simulate the movement of a robot
// moving in 2D. Such robots have 3 DOFS namely the x,y, coordinates
// of the location and the orientation of the robot in the plane. The
// environment has  some distinct landmarks and one more agent that sends
// commands to the moving robot based on the location data that
// it receives from the robot.
    

#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include "utilities/math_constants.h"
#include "utilities/simple_plots.h"

//include functionality for initializing
//and handling the simulator
#include "robosim/configuration/default_configuration.h"
#include "robosim/base/simulator.h"



#include "robosim/base/scalar_variable.h"
#include "robosim/base/vector_variable.h"
#include "robosim/communication/cmd_engine.h"
#include "robosim/engines/electric_engine.h"

//we will simulate a mobile robot. So this will be
//an instance of the MobileAgent class
#include "robosim/ai/mobile_agent.h"

//we will use Ackermann steering. Hence we will be simulating
//a robot that moves in the plain 
#include "robosim/models/ackermann_drive.h"

#include "robosim/maths/Vec.h"

//mrpt related stuff
#include <mrpt/gui/CDisplayWindowPlots.h>
#include <mrpt/system/os.h>
#include <mrpt/system/threads.h>



using namespace std;
using namespace robosim;


namespace exe{
 
//the configuration that the simulator is using   
typedef Configuration<std::vector<AgentBase*>,
                      parframepp::serial> config_type;



//the robot that we simulate
class MyRobot: public MobileAgent<AckermannDrive<OdometryME<2>>>
{

	public:
		
		//ctor
		MyRobot()
		{}

		//any actions that the robot must do before
		//acting. For our case this is empty.
	    	virtual void actions_before_act()final{};
    
		//the robot simply integrates its position and orientation 
		virtual void act()final;
    
		//after integrating position and orientation
		//report them to the terminal
    		virtual void actions_after_act()final{}

};


void
MyRobot::act(){

		//get the current global position
		const VectorVariable& pos = this->get_position();

		Vec<2,real_type> first_goal;
		first_goal[0] = 5.0;
		first_goal[1] = 0.0;

		Vec<2,real_type> second_goal;
		second_goal[0] = 5.0;
		second_goal[1] = 5.0;

		static bool reached_first_goal = false;
		static bool reached_second_goal = false;

		if(std::fabs(pos(0,0)-first_goal[0])<=1.0e-1 && !reached_first_goal){

			real_type dt = SimData::get_dt(); 

			//calculate the steering angle
			//get the local speed
			real_type local_speed = this->compute_speed();

			//get the base length of the wheels
			real_type L = this->get_wheel_base_distance();

			real_type C = (dt*local_speed)/L;

			//we want to turn off the increment on the x-coordinate
			//the increment on the x-ccordinate is given by
			//dt*V*cos(theta). The steering angle must be such that
			//the cosine gives zero. The theta angle
			//is zero at the beginning of this example. Thus, we want to
			//make it close to pi/2. The increment for theta is:
			//
			// dtheta = (dt*V*tan(alpha))/L
			//
			//Hence, we can have the equation of alpha
			real_type a = std::atan(utilities::MathConstants::Pi()/(2.0*C));

			this->set_steering_angle(a);
			reached_first_goal = true;
		}
		else if(std::fabs(pos(0,1)-second_goal[1])<=1.0e-1 && !reached_second_goal){

			real_type dt = SimData::get_dt(); 

			//get the current orientation of the robot
			real_type theta = this->get_state().theta();

			real_type local_speed = this->compute_speed();

			//get the base length of the wheels
			real_type L = this->get_wheel_base_distance();

			real_type C = (dt*local_speed)/L;

			//we want to turn off the increment on the x-coordinate
			//
			//if we are in here this means the robot is facing north. We want
			// to set the orientation back to zero meaning facing East.
			// Since now theta = pi/2 we must set theta = 0
			//
			//Thus, by using the increment for theta dtheta = (dt*V*tan(alpha))/L
			//then we must have dtheta = -theta. Thus, we have an equation to solve
			//for the steering angle
			real_type a = std::atan(-theta/C);

			this->set_steering_angle(a);
			reached_second_goal = true;
		}
		else{

			this->set_steering_angle(0.0);
		}

		//use the Ackermann kinematic model to update
		//position and orientation
		this->execute(this->get_state());

		//after executing report to the console the state
		this->get_state().report(std::cout);
}





//Helper class that wraps functionality about the
//simulation
class Simulation{
    
    
public:
    
    Simulation(const size_type niterations)
    :
    nitrs_(niterations)
    {}
    
    
    //run the simulation
    void run();
       
private:
    
	//initializations for the mobile agent
	void initialize_mobile_agent(MyRobot& robot);
    
    	//number of time steps to run the simulation
    	size_type nitrs_;
};



void
Simulation::run(){
    
	//add a robot to the global simulator
    	MyRobot* robot = static_cast<MyRobot*>(Simulator<config_type>::add_agent(new MyRobot()));


	//create a window that shows the results of the simulation
    	//object that handles the plotting window
    	mrpt::gui::CDisplayWindowPlots window("Ackermann Drive rectilinear motion demo",450,400);
    	window.setPos(100,100);
    	window.axis(-2,20,-10,10); 
    	window.axis_equal();
    
    
    	//print how many robots the simulato has
    	std::cout<<"Number of agents: "<<Simulator<config_type>::n_agents()<<std::endl;
    
    	initialize_mobile_agent(*robot);

	const real_type dt = SimData::get_dt();
	std::cout<<"Starting simulation..."<<std::endl;
	std::cout<<"Time step used: "<<dt<<std::endl;

	//create a file that registers the position at every time step
	utilities::create_file("Positions.csv");	

	size_type itr=0;
	while(itr != nitrs_ && !mrpt::system::os::kbhit()){

		std::cout<<"Time now is: "<<SimData::get_global_time()<<std::endl;
    
        	//do one iteration
        	Simulator<config_type>::iterate_once();

		const VectorVariable& pos = robot->get_position();

		std::pair<real_type,real_type> positions = std::make_pair(pos(0,0),pos(0,1));

		//write to the file the current position
		utilities::append_to_file("Positions.csv",positions,utilities::CSV());

		vector<float> px(2,0.0);
		vector<float> py(2,0.0);
		px[0] = pos(0,0);  
		py[0] = pos(0,1);
		
		window.plot( px,py, "g-4", "Position");
        
		// Delay:
		mrpt::system::sleep(static_cast<int>(dt*1000));

		//increase the global time by dt
        	SimData::add_global_time(dt);
		itr++;
	}    	 
}

void 
Simulation::initialize_mobile_agent(MyRobot& robot){
    
    	//add an engine to the robot
    	robot.add_engine(new ElectricEngine());
    
    	//create a state
    	StateBase state;
    
    	//set position. The robot starts at (0,0)
    	VectorVariable position("Position",1,"m");
    	position.set_number_of_values(2);
    
    	state.set_position(position);
    
    	//set orientation angle. This is a 2d mobile robot
    	//hence the rest angles don't matter
    	state.set_psi(0.0);
     
    	//set the state of the robot
    	robot.set_state(state);

	//now set the data for the Ackermann drive system
	//we must set the wheels radius, the wheel base
	//distance, the steering angle and the wheels speed
	robot.set_wheel_radius(0.3); //r = 0.3 m
	robot.set_wheel_base_distance(0.6); //L = 0.6 m
	robot.set_w(2.0); // w= 2.0 rad/sec
	robot.set_steering_angle(0.0); //alpha = 0 degrees no steering 
    
}

}//exe

int main(int argc, char** argv) {
    
    //inlitialize the simulator
    Simulator<exe::config_type>::initialize(nullptr);
    
    const size_type nitrs = 200;
    exe::Simulation sim(nitrs);
    
    sim.run();
    
    Simulator<exe::config_type>::finalize();
    
   
    return 0;
}

