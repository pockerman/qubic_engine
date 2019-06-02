/* 
 * File:   exe.cpp
 * Author: david
 *
 * Created on October 10, 2015, 12:28 PM
 */

#include "robosim/grids/base_grid_cell.h"
#include "robosim/grids/occupancy_grid.h"
#include "robosim/grids/occupancy_grid_creator.h"
#include <iostream>

using namespace robosim;

namespace exe
{  
    OccupancyGrid grid;
    arma::vec low={0.0,0.0};
    arma::vec high = {1.0,1.0};
    size_type Nx = 5;
    size_type Ny = 5;
    std::vector<real_type> probs;   
}


int main(int argc, char** argv) {
    
    
    using namespace exe;
    OccupancyGridCreator::build(low,high,Nx,Ny,probs,grid);
    
    //loop over the cells of the grid and check
    //the neighbors
    for(size_type c=0; c < grid.n_cells(); ++c){
        
        const BaseGridCell* cell = grid.cell(c);
       
        cell->print(std::cout);    
    }
   
    return 0;
}

