/* 
 * File:   occupancy_grid_creator.h
 * Author: david
 *
 * Created on October 25, 2016, 4:02 PM
 */

#ifndef OCCUPANCY_GRID_CREATOR_H
#define	OCCUPANCY_GRID_CREATOR_H

#include "robosim/base/robosim.h"
#include <armadillo>
#include <vector>

namespace robosim
{
 
//forward declarations
class OccupancyGrid;
    
/**
 * @brief Helper class to build rectangular occupancy grids
 */
class OccupancyGridCreator
{
public:
    
    static void build(const arma::vec& low, const arma::vec& high,
                      size_type Nx,size_type Ny,
                      OccupancyGrid& grid);
    
    static void build(const arma::vec& low, const arma::vec& high,
                      size_type Nx,size_type Ny,std::vector<real_type>& probs,
                      OccupancyGrid& grid);
    
    
    
};
    
    
}



#endif	/* OCCUPANCY_GRID_CREATOR_H */

