/* 
 * File:   exe.cpp
 * Author: david
 *
 * Created on October 10, 2015, 12:28 PM
*/

#include "exe.h"
#include "robosim/grids/occupancy_grid_creator.h"
#include "robosim/plans/bfs_location_plan_builder.h"
#include "robosim/simulator/serial_simulator.h"
#include "robosim/grids/base_grid_cell.h"

#include "utilities/math_constants.h"

//for rendering the simulation
//with OpenGL
#include "glut.h"

namespace exe2
{


RobotAI::RobotAI()
:
grid_(),
plan_(),
goal_reached_(false),
theta_d_(0.0),
Vr_d_(0.5),
Vl_d_(0.5)
{}

void
RobotAI::set_up_occupancy_grid(){

    size_type ncells = 25;
    std::vector<real_type> probs(ncells,0.0);
    probs[1] = 1.0;
    probs[3] = 1.0;
    probs[6] = 1.0;
    probs[8] = 1.0;
    probs[13] = 1.0;
    probs[16] = 1.0;
    probs[21] = 1.0;
    probs[23] = 1.0;
        
    //generate the grid
    arma::vec low = {0.0,0.0};
    arma::vec high = {5.0,5.0};
    OccupancyGridCreator::build(low,high,5,5,probs,grid_);
}

void 
RobotAI::create_location_plan(){

  //build the plan
  BFSLocationPlanBuilder::build_shortest_path(grid_,plan_,20,4);
  
  //set the start and goal locations of the plan
  plan_.set_start_location(grid_.cell(20)->get_centroid());
  plan_.set_goal_location(grid_.cell(4)->get_centroid());

}

void 
RobotAI::generate_cmds(const State<2>& state){
    
    //if we are at the goal then
    //we need to stop
    if(state.distance(plan_.get_goal()) < RoboSimCommon::tolerance()){
        
        goal_reached_ = true;
        return; 
    }
    
    if(current_seg_.id==0){
        
        real_type dis = state.distance(grid_.cell(10)->get_centroid());
        std::cout<<"\tDistance from first landmark: "<<dis
                <<" at tolerance: "<<RoboSimCommon::tolerance()<<std::endl;
        
        //the robot moves at the first segment
        if(dis < RoboSimCommon::tolerance()){
            
            std::cout<<"\tTurning left at first landmark"<<std::endl;
            current_seg_.id = 1;
            theta_d_ = 0.0;
            return;
        }
        else{
            theta_d_ = 3.0*utilities::MathConstants::Pi()*0.5;
            return;
        }
    }
    
    if(current_seg_.id==1){
        
        real_type dis = state.distance(grid_.cell(12)->get_centroid());
        std::cout<<"\tDistance from second landmark: "<<dis
                <<" at tolerance: "<<RoboSimCommon::tolerance()<<std::endl;
        
        if(dis < RoboSimCommon::tolerance()){
            
            std::cout<<"\tTurning left at second landmark"<<std::endl;
            current_seg_.id = 2;
            theta_d_ = utilities::MathConstants::Pi()*0.5;
            return;
        }
        else{
            theta_d_ = 0.0;
            return;
        }
    }
    
    if(current_seg_.id == 2){
        
        real_type dis = state.distance(grid_.cell(17)->get_centroid());
        std::cout<<"\tDistance from third landmark: "<<dis
                <<" at tolerance: "<<RoboSimCommon::tolerance()<<std::endl;
        
        if(dis < RoboSimCommon::tolerance()){
            std::cout<<"\tTurning right at third landmark"<<std::endl;
            current_seg_.id = 3;
            theta_d_ = 0.0;
            return;
        }
        else{
            theta_d_ = utilities::MathConstants::Pi()*0.5;
            return;
        }
    }
    
    if(current_seg_.id == 3){
        
        real_type dis = state.distance(grid_.cell(19)->get_centroid());
        std::cout<<"\tDistance from fourth landmark: "<<dis
                <<" at tolerance: "<<RoboSimCommon::tolerance()<<std::endl;
        
        if(state.distance(grid_.cell(19)->get_centroid())< RoboSimCommon::tolerance()){
            std::cout<<"\tTurning right at fourth landmark"<<std::endl;
            current_seg_.id = 4;
            theta_d_ = 3.0*utilities::MathConstants::Pi()*0.5;
            return;
        }
    }    
}

void 
Robot::execute(){

    if(!has_grid_and_plan_){

        this->get_ai().set_up_occupancy_grid();
        this->get_ai().create_location_plan();
        this->get_ai().current_seg_.id=0;
        
        //this is where we set up the initial
        //position of the robot. We get the start location
        //from the plan
        arma::vec& start_pos = this->get_ai().plan_.get_start();
        State<2> init;
        init.set_position(start_pos);
        init.set_theta(3.0*utilities::MathConstants::Pi()*0.5);
        
        //set the old state
        this->get_state().set_old_state(init);
        this->get_chassis().set_wheels_speeds(this->get_ai().Vr_d_,this->get_ai().Vl_d_);
        has_grid_and_plan_ = true;
    }
    
    this->get_ai().generate_cmds(this->get_state());
    
    if(this->get_ai().goal_reached_){
      
        std::cout<<"Goal reached..."<<std::endl;
        return;
    }
    
    //now that we generated the commands
    //set properly the desired values
    std::cout<<"\tVr/Vl: "<<this->get_ai().Vr_d_<<" "<<this->get_ai().Vl_d_<<std::endl;
    this->get_state().get_old_state().set_theta(this->get_ai().theta_d_);
    
    //let's make the updates
    this->update_state();
    
    this->get_state().print(std::cout);
    //this->get_state().save_current_state("state");
}

}

int main(int argc, char** argv) {
   
    using namespace exe2;

    try{
        
        
	std::cout<<"Starting simulation..."<<std::endl;

	//run the simulation for 5 steps with dt = 0.01
	SerialSimulator::init(22,0.01);

	//add the model to simulate
	Robot* rptr = SerialSimulator::add_model<Robot>();
        
	SerialSimulator::run();

	std::cout<<"Simulation finished..."<<std::endl;
	SerialSimulator::finalize();
    }
    catch(utilities::ExeInvalidIndex& e){
	std::cerr<<e.what()<<std::endl;
    }
    catch(...){
	std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }
   
  
    return 0;
}

