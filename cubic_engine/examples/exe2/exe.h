/* 
 * File:   exe.h
 * Author: david
 *
 * Created on October 25, 2015, 10:26 AM
 */

#ifndef EXE_H
#define	EXE_H

#include "robosim/base/robosim.h"
#include "robosim/models/ground_moving_vehicle/differential_drive_vehicle.h"
#include "robosim/models/chassis/pioneer_3dx.h"
#include "robosim/grids/occupancy_grid.h"
#include "robosim/plans/location_plan.h"



#include <map>

using namespace std;
using namespace robosim;
using namespace utilities;

namespace exe2{
    
//forward declarations
class Robot;

struct LineSegment
{
    
    bool line_started;
    bool line_finished;
    size_type id;
    
};
    
//The object that makes the decisions
//about the robot
class RobotAI
{

public:

	//Constructor
	RobotAI();

	//set up the occupancy grid
	void set_up_occupancy_grid();


	//create the static location plan
	void create_location_plan();
  
  //generate heading and left and right
  //wheels speeds 
  void generate_cmds(const State<2>& state);
  
private:

  //the object that holds the info
  //about the current line segment the
  //robot is on
  LineSegment current_seg_;  

	//The static occupancy grid map
	OccupancyGrid grid_;

	//the static location plan
	LocationPlan2D plan_;
  
  //make friends with the robot class
  friend class Robot;
  
  //flag indicating the goal has been reached
  bool goal_reached_;
  
  //brief the desired heading
  real_type theta_d_;
    
  //the desired right wheel velocity
  real_type Vr_d_;
    
  //the desired left wheel velocity
  real_type Vl_d_;
	
};

//the robot class
class Robot: public DifDriveVehicle<RobotAI,Pioneer3DX>
{


public:

	//Constructor
	Robot(size_type id);

	//perform any calculations that the robot must perform
	virtual void execute();
  
protected:
    
    //flag indicating that the grid and the
    //plan have been generated. This has to be 
    //done only once as we use a static grid
    //and static location plan
    bool has_grid_and_plan_ =false;
    
    
    

};


inline
Robot::Robot(size_type id)
:
DifDriveVehicle<RobotAI,Pioneer3DX>(id)
{}



  
 

    
}//exe2



#endif	/* EXE_H */

