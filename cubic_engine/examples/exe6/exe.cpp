/* 
 * File:   exe.cpp
 * Author: david
 *
 * Created on October 10, 2015, 12:28 PM
 */

/**

  In previous examples (4,5) we saw how we can launch a simple
  simulation for a mobile robot. In these examples, most of the
  functionality was hardcoded somehow. In this example, we discuss how
  we can implement a decision system for our robot. In particular, we continue
  from example 5. We will use a SLAM algorithm to navigate our robot. Concretely,
  we will use the ICP Slam 

*/
    

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

#include <mrpt/maps/CSimplePointsMap.h>
#include <mrpt/obs/CObservation2DRangeScan.h>
#include <mrpt/slam/CICP.h>
#include <mrpt/poses/CPose2D.h>
#include <mrpt/poses/CPosePDF.h>
#include <mrpt/poses/CPosePDFGaussian.h>
#include <mrpt/gui.h>
#include <mrpt/math/utils.h>



using namespace std;
using namespace robosim;
using namespace mrpt;
using namespace mrpt::utils;
using namespace mrpt::slam;
using namespace mrpt::maps;
using namespace mrpt::obs;
using namespace mrpt::math;
using namespace mrpt::poses;


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





//first we need to learn ICP Slam


bool skip_window=false;
int  ICP_method = (int) icpClassic;

#define SCANS_SIZE 361

#if 0
	// Hard matching in a corridor
	float SCAN_RANGES_1[] = { 2.670f,2.670f,2.670f,2.670f,2.670f,2.660f,2.670f,2.670f,2.670f,2.670f,2.670f,2.670f,2.670f,2.670f,2.670f,2.670f,2.680f,2.680f,2.680f,2.680f,2.690f,2.690f,2.700f,2.690f,2.700f,2.700f,2.710f,2.710f,2.720f,2.720f,2.720f,2.730f,2.740f,2.730f,2.740f,2.750f,2.750f,2.760f,2.770f,2.780f,2.780f,2.790f,2.800f,2.800f,2.820f,2.820f,2.830f,2.840f,2.850f,2.860f,2.870f,2.880f,2.890f,2.900f,2.910f,2.920f,2.930f,2.940f,2.950f,2.970f,2.990f,2.990f,3.010f,3.020f,3.040f,3.050f,3.070f,3.080f,3.100f,3.120f,3.140f,3.150f,3.170f,3.180f,3.200f,3.220f,3.240f,3.250f,3.280f,3.300f,3.320f,3.340f,3.370f,3.390f,3.420f,3.440f,3.470f,3.490f,3.510f,3.540f,3.570f,3.590f,3.630f,3.650f,3.690f,3.710f,3.750f,3.770f,3.820f,3.850f,3.890f,3.920f,3.960f,3.990f,4.030f,4.060f,4.120f,4.160f,4.210f,4.230f,4.290f,4.310f,4.380f,4.420f,4.490f,4.530f,4.590f,4.640f,4.710f,4.760f,4.830f,4.890f,4.970f,5.010f,5.110f,5.170f,5.260f,5.330f,5.410f,5.490f,5.590f,5.670f,5.780f,5.860f,5.980f,6.070f,6.180f,6.300f,6.410f,6.530f,6.670f,6.790f,6.950f,7.100f,7.250f,7.400f,7.590f,7.750f,7.940f,8.140f,8.380f,8.580f,8.840f,9.060f,9.360f,9.610f,9.940f,10.210f,10.600f,10.930f,11.380f,11.750f,12.280f,12.730f,13.300f,13.850f,14.580f,15.210f,16.040f,16.820f,17.890f,18.840f,20.210f,21.470f,23.260f,81.870f,81.870f,81.910f,81.910f,81.910f,39.910f,39.940f,81.870f,	81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.870f,81.910f,81.870f,81.870f,21.120f,21.120f,21.180f,21.190f,21.250f,19.140f,21.310f,14.710f,14.170f,13.680f,13.130f,12.690f,12.230f,11.860f,11.450f,11.140f,10.800f,10.520f,10.180f,9.920f,9.650f,9.440f,9.170f,8.980f,8.740f,8.570f,8.360f,8.180f,7.990f,7.850f,7.660f,7.540f,7.380f,7.260f,7.110f,6.990f,6.860f,6.760f,6.620f,6.530f,6.410f,6.310f,6.200f,6.120f,6.020f,5.940f,5.840f,5.780f,5.680f,5.620f,5.520f,5.460f,5.380f,5.320f,5.250f,5.200f,5.130f,5.080f,5.010f,4.960f,4.900f,4.860f,4.790f,4.760f,4.700f,4.660f,4.610f,4.570f,4.520f,4.490f,4.430f,4.400f,4.350f,4.320f,4.280f,4.240f,4.200f,4.180f,4.140f,4.120f,4.070f,4.050f,4.010f,3.990f,3.950f,3.930f,3.900f,3.870f,3.850f,3.820f,3.790f,3.770f,3.740f,3.720f,3.700f,3.670f,3.650f,3.640f,3.610f,3.590f,3.570f,3.560f,3.530f,3.520f,3.500f,3.490f,3.460f,3.450f,3.430f,3.420f,3.400f,3.400f,3.380f,3.360f,3.360f,3.330f,3.320f,3.310f,3.300f,3.290f,3.270f,3.260f,3.250f,3.240f,3.230f,3.220f,3.210f,3.200f,3.200f,3.190f,3.180f,3.170f,3.160f,3.150f,3.150f,3.150f,3.140f,3.140f,3.120f,3.120f,3.110f,3.100f,3.100f,3.100f,3.100f,3.090f,3.080f,3.080f,3.080f,3.080f,3.070f,3.070f,3.070f,3.060f,3.070f,3.060f,3.060f };
	char  SCAN_VALID_1[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

	float SCAN_RANGES_2[] = {2.630f,2.630f,2.630f,2.630f,2.630f,2.630f,2.630f,2.630f,2.630f,2.630f,2.630f,2.630f,2.630f,2.630f,2.640f,2.640f,2.640f,2.640f,2.640f,2.640f,2.650f,2.650f,2.650f,2.650f,2.660f,2.660f,2.670f,2.670f,2.670f,2.670f,2.680f,2.680f,2.690f,2.690f,2.700f,2.710f,2.710f,2.710f,2.720f,2.730f,2.740f,2.750f,2.750f,2.760f,2.770f,2.780f,2.790f,2.800f,2.800f,2.820f,2.830f,2.840f,2.850f,2.860f,2.870f,2.880f,2.890f,2.900f,2.920f,2.930f,2.930f,2.950f,2.960f,2.980f,2.990f,3.000f,3.020f,3.040f,3.060f,3.070f,3.090f,3.110f,3.120f,3.140f,3.150f,3.170f,3.190f,3.200f,3.230f,3.250f,3.270f,3.290f,3.320f,3.330f,3.360f,3.380f,3.410f,3.430f,3.460f,3.490f,3.510f,3.540f,3.570f,3.590f,3.620f,3.650f,3.680f,3.720f,3.750f,3.750f,3.800f,3.850f,3.880f,3.930f,3.960f,3.990f,4.040f,4.060f,4.110f,4.160f,4.210f,4.250f,4.300f,4.330f,4.410f,4.450f,4.500f,4.550f,4.610f,4.660f,4.730f,4.780f,4.860f,4.930f,4.990f,5.060f,5.140f,5.210f,5.300f,5.380f,5.450f,5.540f,5.630f,5.730f,5.820f,5.920f,6.030f,6.130f,6.240f,6.360f,6.490f,6.620f,6.760f,6.900f,7.050f,7.210f,7.370f,7.540f,7.730f,7.920f,8.120f,8.330f,8.550f,8.780f,9.040f,9.300f,9.580f,9.880f,10.230f,10.580f,10.940f,11.340f,11.780f,12.240f,12.740f,13.300f,13.910f,14.580f,15.260f,16.030f,16.930f,17.900f,19.060f,20.300f,81.870f,23.500f,81.910f,81.910f,81.910f,81.910f,81.910f,38.760f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.910f,81.870f,81.910f,20.620f,20.000f,20.020f,20.050f,20.070f,17.870f,20.140f,17.950f,20.210f,13.560f,13.160f,12.700f,12.240f,11.850f,11.490f,11.120f,10.840f,10.510f,10.230f,9.940f,9.700f,9.440f,9.220f,8.990f,8.790f,8.570f,8.390f,8.210f,8.030f,7.860f,7.710f,7.560f,7.420f,7.270f,7.150f,7.020f,6.900f,6.770f,6.660f,6.550f,6.460f,6.340f,6.260f,6.150f,6.060f,5.980f,5.890f,5.800f,5.730f,5.650f,5.580f,5.500f,5.430f,5.360f,5.300f,5.230f,5.170f,5.100f,5.060f,5.000f,4.940f,4.880f,4.830f,4.770f,4.730f,4.690f,4.640f,4.600f,4.550f,4.520f,4.480f,4.420f,4.390f,4.340f,4.310f,4.280f,4.240f,4.200f,4.180f,4.150f,4.120f,4.070f,4.050f,4.010f,3.990f,3.960f,3.940f,3.910f,3.880f,3.850f,3.830f,3.800f,3.780f,3.760f,3.740f,3.710f,3.690f,3.660f,3.640f,3.620f,3.600f,3.580f,3.570f,3.550f,3.540f,3.520f,3.510f,3.490f,3.470f,3.450f,3.440f,3.430f,3.410f,3.400f,3.390f,3.380f,3.380f,3.350f,3.350f,3.340f,3.330f,3.290f,3.290f,3.270f,3.260f,3.250f,3.240f,3.240f,3.230f,3.220f,3.210f,3.200f,3.200f,3.180f,3.180f,3.180f,3.180f,3.160f,3.150f,3.150f,3.150f,3.130f,3.130f,3.120f,3.130f,3.120f,3.140f,3.130f,3.130f,3.110f,3.120f,3.120f,3.120f,3.100f,3.100f,3.090f};
	char  SCAN_VALID_2[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
#else
	// A more usual case:
	float SCAN_RANGES_1[] = {0.910f,0.900f,0.910f,0.900f,0.900f,0.890f,0.890f,0.880f,0.890f,0.880f,0.880f,0.880f,0.880f,0.880f,0.880f,0.870f,0.880f,0.870f,0.870f,0.870f,0.880f,0.880f,0.880f,0.880f,0.880f,0.880f,0.880f,0.880f,0.880f,0.880f,0.880f,0.880f,0.880f,0.880f,0.880f,0.880f,0.890f,0.880f,0.880f,0.880f,0.890f,0.880f,0.890f,0.890f,0.880f,0.890f,0.890f,0.880f,0.890f,0.890f,0.890f,0.890f,0.890f,0.890f,0.900f,0.900f,0.900f,0.900f,0.900f,0.910f,0.910f,0.910f,0.910f,0.920f,0.920f,0.920f,0.920f,0.920f,0.930f,0.930f,0.930f,0.930f,0.940f,0.940f,0.950f,0.950f,0.950f,0.950f,0.960f,0.960f,0.970f,0.970f,0.970f,0.980f,0.980f,0.990f,1.000f,1.000f,1.000f,1.010f,1.010f,1.020f,1.030f,1.030f,1.030f,1.040f,1.050f,1.060f,1.050f,1.060f,1.070f,1.070f,1.080f,1.080f,1.090f,1.100f,1.110f,1.120f,1.120f,1.130f,1.140f,1.140f,1.160f,1.170f,1.180f,1.180f,1.190f,1.200f,1.220f,1.220f,1.230f,1.230f,1.240f,1.250f,1.270f,1.280f,1.290f,1.300f,1.320f,1.320f,1.350f,1.360f,1.370f,1.390f,1.410f,1.410f,1.420f,1.430f,1.450f,1.470f,1.490f,1.500f,1.520f,1.530f,1.560f,1.580f,1.600f,1.620f,1.650f,1.670f,1.700f,1.730f,1.750f,1.780f,1.800f,1.830f,1.850f,1.880f,1.910f,1.940f,1.980f,2.010f,2.060f,2.090f,2.130f,2.180f,2.220f,2.250f,2.300f,2.350f,2.410f,2.460f,2.520f,2.570f,2.640f,2.700f,2.780f,2.850f,2.930f,3.010f,3.100f,3.200f,3.300f,3.390f,3.500f,3.620f,3.770f,3.920f,4.070f,4.230f,4.430f,4.610f,4.820f,5.040f,5.290f,5.520f,8.970f,8.960f,8.950f,8.930f,8.940f,8.930f,9.050f,9.970f,9.960f,10.110f,13.960f,18.870f,19.290f,81.910f,20.890f,48.750f,48.840f,48.840f,19.970f,19.980f,19.990f,15.410f,20.010f,19.740f,17.650f,17.400f,14.360f,12.860f,11.260f,11.230f,8.550f,8.630f,9.120f,9.120f,8.670f,8.570f,7.230f,7.080f,7.040f,6.980f,6.970f,5.260f,5.030f,4.830f,4.620f,4.440f,4.390f,4.410f,4.410f,4.410f,4.430f,4.440f,4.460f,4.460f,4.490f,4.510f,4.540f,3.970f,3.820f,3.730f,3.640f,3.550f,3.460f,3.400f,3.320f,3.300f,3.320f,3.320f,3.340f,2.790f,2.640f,2.600f,2.570f,2.540f,2.530f,2.510f,2.490f,2.490f,2.480f,2.470f,2.460f,2.460f,2.460f,2.450f,2.450f,2.450f,2.460f,2.460f,2.470f,2.480f,2.490f,2.490f,2.520f,2.510f,2.550f,2.570f,2.610f,2.640f,2.980f,3.040f,3.010f,2.980f,2.940f,2.920f,2.890f,2.870f,2.830f,2.810f,2.780f,2.760f,2.740f,2.720f,2.690f,2.670f,2.650f,2.630f,2.620f,2.610f,2.590f,2.560f,2.550f,2.530f,2.510f,2.500f,2.480f,2.460f,2.450f,2.430f,2.420f,2.400f,2.390f,2.380f,2.360f,2.350f,2.340f,2.330f,2.310f,2.300f,2.290f,2.280f,2.270f,2.260f,2.250f,2.240f,2.230f,2.230f,2.220f,2.210f,2.200f,2.190f,2.180f,2.170f,1.320f,1.140f,1.130f,1.130f,1.120f,1.120f,1.110f,1.110f,1.110f,1.110f,1.100f,1.110f,1.100f};
	char  SCAN_VALID_1[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

	float SCAN_RANGES_2[] = {0.720f,0.720f,0.720f,0.720f,0.720f,0.720f,0.710f,0.720f,0.710f,0.710f,0.710f,0.710f,0.710f,0.710f,0.710f,0.710f,0.710f,0.710f,0.710f,0.710f,0.720f,0.720f,0.720f,0.720f,0.730f,0.730f,0.730f,0.730f,0.730f,0.730f,0.730f,0.740f,0.740f,0.740f,0.740f,0.740f,0.740f,0.750f,0.750f,0.750f,0.750f,0.750f,0.750f,0.750f,0.750f,0.760f,0.760f,0.760f,0.760f,0.760f,0.760f,0.760f,0.760f,0.770f,0.770f,0.770f,0.770f,0.780f,0.780f,0.780f,0.790f,0.790f,0.800f,0.800f,0.800f,0.800f,0.800f,0.800f,0.810f,0.810f,0.820f,0.820f,0.830f,0.830f,0.840f,0.840f,0.850f,0.850f,0.860f,0.860f,0.860f,0.870f,0.870f,0.880f,0.890f,0.890f,0.900f,0.900f,0.910f,0.920f,0.930f,0.930f,0.940f,0.940f,0.940f,0.950f,0.960f,0.960f,0.970f,0.980f,0.990f,1.000f,1.010f,1.020f,1.030f,1.040f,1.050f,1.060f,1.070f,1.080f,1.080f,1.100f,1.100f,1.120f,1.120f,1.140f,1.140f,1.170f,1.160f,1.180f,1.190f,1.210f,1.220f,1.240f,1.250f,1.280f,1.290f,1.300f,1.320f,1.340f,1.350f,1.380f,1.390f,1.420f,1.440f,1.460f,1.470f,1.500f,1.520f,1.550f,1.570f,1.600f,1.630f,1.670f,1.690f,1.730f,1.760f,1.790f,1.820f,1.870f,1.900f,1.940f,1.970f,2.030f,2.080f,2.130f,2.170f,2.230f,2.280f,2.340f,2.400f,2.490f,2.550f,2.630f,2.700f,2.810f,2.880f,3.010f,3.090f,3.240f,3.340f,3.500f,3.620f,3.810f,3.950f,4.180f,4.340f,4.620f,8.170f,8.140f,8.150f,8.120f,8.110f,8.100f,8.100f,8.300f,9.040f,9.130f,9.130f,13.030f,18.050f,19.150f,81.910f,20.070f,47.980f,48.040f,48.030f,19.140f,19.180f,19.180f,19.190f,14.550f,19.210f,16.850f,16.840f,7.800f,7.770f,7.770f,7.750f,7.770f,7.760f,7.780f,7.760f,8.320f,8.350f,8.350f,8.090f,7.720f,7.730f,6.430f,6.360f,6.290f,6.260f,6.230f,6.220f,6.160f,5.800f,4.510f,4.410f,4.240f,4.140f,4.000f,3.910f,3.790f,3.680f,3.660f,3.680f,3.680f,3.700f,3.710f,3.730f,3.730f,3.760f,3.770f,3.790f,3.820f,3.850f,3.900f,3.940f,3.980f,3.250f,3.180f,3.140f,3.070f,3.030f,2.970f,2.930f,2.880f,2.850f,2.790f,2.760f,2.710f,2.680f,2.660f,2.670f,2.690f,2.710f,2.720f,2.740f,2.760f,2.770f,2.780f,2.800f,2.170f,2.120f,2.090f,2.060f,2.020f,2.010f,1.990f,1.980f,1.970f,1.960f,1.950f,1.950f,1.940f,1.940f,1.950f,1.940f,1.940f,1.950f,1.930f,1.940f,1.940f,1.940f,1.940f,1.940f,1.950f,1.960f,1.960f,1.980f,1.980f,2.000f,2.010f,2.030f,2.060f,2.090f,2.120f,2.190f,2.560f,2.540f,2.530f,2.520f,2.500f,2.490f,2.470f,2.460f,2.450f,2.440f,2.420f,2.410f,2.400f,2.390f,2.380f,2.370f,2.360f,2.350f,2.340f,2.340f,2.330f,2.320f,2.310f,2.300f,2.290f,2.290f,2.290f,2.280f,2.270f,2.260f,2.260f,2.250f,2.240f,2.240f,2.230f,2.230f,2.220f,2.220f,2.210f,2.210f,2.200f,2.200f,2.190f,2.190f,2.190f,2.180f,2.180f,2.170f,2.170f,2.170f,2.160f,2.160f};
	char  SCAN_VALID_2[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

#endif

// ------------------------------------------------------
//				TestICP
// ------------------------------------------------------
void TestICP()
{
	CSimplePointsMap		m1,m2;
	float				runningTime;
	CICP::TReturnInfo		info;
	CICP				ICP;

	// Load scans:
	CObservation2DRangeScan	scan1;
	scan1.aperture = M_PIf;
	scan1.rightToLeft = true;
	scan1.validRange.resize( SCANS_SIZE );
	scan1.scan.resize(SCANS_SIZE);

	ASSERT_( sizeof(SCAN_RANGES_1) == sizeof(float)*SCANS_SIZE );

	memcpy( &scan1.scan[0], SCAN_RANGES_1, sizeof(SCAN_RANGES_1) );
	memcpy( &scan1.validRange[0], SCAN_VALID_1, sizeof(SCAN_VALID_1) );

	CObservation2DRangeScan	scan2 = scan1;
	memcpy( &scan2.scan[0], SCAN_RANGES_2, sizeof(SCAN_RANGES_2) );
	memcpy( &scan2.validRange[0], SCAN_VALID_2, sizeof(SCAN_VALID_2) );

	// Build the points maps from the scans:
	m1.insertObservation( &scan1 );
	m2.insertObservation( &scan2 );

#if MRPT_HAS_PCL
	cout << "Saving map1.pcd and map2.pcd in PCL format...\n";
	m1.savePCDFile("map1.pcd", false);
	m2.savePCDFile("map2.pcd", false);
#endif

	// -----------------------------------------------------
//	ICP.options.ICP_algorithm = icpLevenbergMarquardt;
//	ICP.options.ICP_algorithm = icpClassic;
	ICP.options.ICP_algorithm = (TICPAlgorithm)ICP_method;

	ICP.options.maxIterations			= 100;
	ICP.options.thresholdAng			= DEG2RAD(10.0f);
	ICP.options.thresholdDist			= 0.75f;
	ICP.options.ALFA					= 0.5f;
	ICP.options.smallestThresholdDist	= 0.05f;
	ICP.options.doRANSAC = false;

	ICP.options.dumpToConsole();
	// -----------------------------------------------------

	CPose2D		initialPose(0.8f,0.0f,(float)DEG2RAD(0.0f));

	CPosePDFPtr pdf = ICP.Align(
		&m1,
		&m2,
		initialPose,
		&runningTime,
		(void*)&info);

	printf("ICP run in %.02fms, %d iterations (%.02fms/iter), %.01f%% goodness\n -> ",
			runningTime*1000,
			info.nIterations,
			runningTime*1000.0f/info.nIterations,
			info.goodness*100 );

	cout << "Mean of estimation: " << pdf->getMeanVal() << endl<< endl;

	CPosePDFGaussian  gPdf;
	gPdf.copyFrom(*pdf);

	cout << "Covariance of estimation: " << endl << gPdf.cov << endl;

	cout << " std(x): " << sqrt( gPdf.cov(0,0) ) << endl;
	cout << " std(y): " << sqrt( gPdf.cov(1,1) ) << endl;
	cout << " std(phi): " << RAD2DEG(sqrt( gPdf.cov(2,2) )) << " (deg)" << endl;

	//cout << "Covariance of estimation (MATLAB format): " << endl << gPdf.cov.inMatlabFormat()  << endl;

	cout << "-> Saving reference map as scan1.txt" << endl;
	m1.save2D_to_text_file("scan1.txt");

	cout << "-> Saving map to align as scan2.txt" << endl;
	m2.save2D_to_text_file("scan2.txt");

	cout << "-> Saving transformed map to align as scan2_trans.txt" << endl;
	CSimplePointsMap m2_trans = m2;
	m2_trans.changeCoordinatesReference( gPdf.mean );
	m2_trans.save2D_to_text_file("scan2_trans.txt");


	/*cout << "-> Saving MATLAB script for drawing 2D ellipsoid as view_ellip.m" << endl;
	CMatrixFloat COV22 =  CMatrixFloat( CMatrixDouble( gPdf.cov ));
	COV22.setSize(2,2);
	CVectorFloat MEAN2D(2);
	MEAN2D[0] = gPdf.mean.x();
	MEAN2D[1] = gPdf.mean.y();
	{
		ofstream f("view_ellip.m");
		f << math::MATLAB_plotCovariance2D( COV22, MEAN2D, 3.0f);
	}*/


	// If we have 2D windows, use'em:
#if MRPT_HAS_WXWIDGETS

	std::cout<<"MRPT HAS WXWIDGETS...."<<std::endl;
	if (!skip_window)
	{
		gui::CDisplayWindowPlots	win("ICP results");

		// Reference map:
		vector<float>   map1_xs, map1_ys, map1_zs;
		m1.getAllPoints(map1_xs,map1_ys,map1_zs);
		win.plot( map1_xs, map1_ys, "b.3", "map1");

		// Translated map:
		vector<float>   map2_xs, map2_ys, map2_zs;
		m2_trans.getAllPoints(map2_xs,map2_ys,map2_zs);
		win.plot( map2_xs, map2_ys, "r.3", "map2");

		// Uncertainty
		win.plotEllipse(MEAN2D[0],MEAN2D[1],COV22,3.0,"b2", "cov");

		win.axis(-1,10,-6,6);
		win.axis_equal();

		cout << "Close the window to exit" << endl;
		win.waitForKey();
	}
#endif


}


int main(int argc, char** argv) {


    TestICP();
    
    //inlitialize the simulator
    /*Simulator<exe::config_type>::initialize(nullptr);
    
    const size_type nitrs = 200;
    exe::Simulation sim(nitrs);
    
    sim.run();
    
    Simulator<exe::config_type>::finalize();*/
    
   
    return 0;
}

