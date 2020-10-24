/*#include "cubicai/grids/occupancy_grid_creator.h"
#include "cubicai/grids/occupancy_grid.h"
#include "cubicai/grids/base_grid_cell.h"
#include "cubicai/base/robosim_config.h"
#include "cubicai/simulator/simulator_base.h"

#ifdef ROBOSIM_DEBUG
#include "utilities/exceptions.h"
#endif*/

#define BOOST_TEST_MODULE OccupancyGridGreatorTest

//unit testing from boost
#include <boost/test/unit_test.hpp>

/*using namespace robosim;

namespace test_data
{
  
    
    arma::vec low={0.0,0.0};
    arma::vec high = {1.0,1.0};
    size_type Nx = 0;
    size_type Ny = 10;
    std::vector<real_type> probs;
    
}*/

BOOST_AUTO_TEST_CASE(OCCUPANCY_GRID_CREATOR_TEST)
{
    
    /*using namespace test_data;
    
    //initialize properly constants etc
    SimBase::init();
    
    {  
      OccupancyGrid grid;
#ifdef ROBOSIM_DEBUG
      BOOST_CHECK_THROW(OccupancyGridCreator::build(low,high,Nx,Ny,grid),utilities::ExeLogicError); 
#endif
    }
    
    {
      OccupancyGrid grid;
      Nx = 2;
      Ny = 2;
      OccupancyGridCreator::build(low,high,Nx,Ny,grid);
      
      for(size_type c=0; c < grid.n_cells(); ++c){
          
        const BaseGridCell* cell = grid.cell(c);
        const std::vector<size_type>& neighbors = cell->get_neighbors();
        
        //we have 4 neighbors
        BOOST_CHECK(neighbors.size()== static_cast<size_type>(4));
          
        if(c == 0){
              
            BOOST_CHECK(neighbors[0] == RoboSimCommon::iuint());
            BOOST_CHECK(neighbors[1] == static_cast<size_type>(1));
            BOOST_CHECK(neighbors[2] == static_cast<size_type>(2)); 
            BOOST_CHECK(neighbors[3] == RoboSimCommon::iuint());
        }
        if(c == 1){
              
            BOOST_CHECK(neighbors[0] == RoboSimCommon::iuint());
            BOOST_CHECK(neighbors[1] == RoboSimCommon::iuint());
            BOOST_CHECK(neighbors[2] == static_cast<size_type>(3)); 
            BOOST_CHECK(neighbors[3] == static_cast<size_type>(0));
        }
        
        if(c == 2){
              
            BOOST_CHECK(neighbors[0] == static_cast<size_type>(0));
            BOOST_CHECK(neighbors[2] == RoboSimCommon::iuint());
            BOOST_CHECK(neighbors[1] == static_cast<size_type>(3)); 
            BOOST_CHECK(neighbors[3] == RoboSimCommon::iuint());
        }
        
        if(c == 3){
              
            BOOST_CHECK(neighbors[0] == static_cast<size_type>(1));
            BOOST_CHECK(neighbors[2] == RoboSimCommon::iuint());
            BOOST_CHECK(neighbors[3] == static_cast<size_type>(2)); 
            BOOST_CHECK(neighbors[1] == RoboSimCommon::iuint());
        }
        
      }
        
    }
    
     //finalize the simulator
     SimBase::finalize();*/
}
