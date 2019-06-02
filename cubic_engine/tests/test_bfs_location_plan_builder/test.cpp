//#include "robosim/plans/bfs_location_plan_builder.h"
//#include "robosim/plans/location_plan.h"
//#include "robosim/grids/occupancy_grid.h"
//#include "robosim/grids/occupancy_grid_creator.h"
//#include "robosim/simulator/simulator_base.h"

#define BOOST_TEST_MODULE BFSLocationPlanBuilderTest

//unit testing from boost
#include <boost/test/unit_test.hpp>

//using namespace robosim;

/*namespace test_data
{
    
    
    void grid_creator(OccupancyGrid& grid){
        
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
        OccupancyGridCreator::build(low,high,5,5,probs,grid);
        
    }
    
    
}*/

BOOST_AUTO_TEST_CASE(BFS_LOCATION_PLAN_BUILDER_TEST)
{
    
    /*{
        //initialize properly constants etc
        SimBase::init();
        
        //the location plan
        LocationPlan2D plan;
        
        //the occupancy grid to use
        OccupancyGrid grid;
        
        //generate the grid
        test_data::grid_creator(grid);
        
        //build the plan
        BFSLocationPlanBuilder::build_shortest_path(grid,plan,20,4);
        
        //finalize the simulator
        SimBase::finalize();
   
    }*/
}
