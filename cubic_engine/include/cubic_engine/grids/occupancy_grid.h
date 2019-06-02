/* 
 * File:   occupancy_structured_grid.h
 * Author: david
 *
 * Created on October 23, 2016, 12:15 PM
 */

#ifndef OCCUPANCY_GRID_H
#define	OCCUPANCY_GRID_H


#include "robosim/grids/base_grid.h"

namespace robosim
{
    
//forward declarations
class BaseGridCell;
class CellVertex;
    
/**
 * @brief A structured occupancy grid
 */
class OccupancyGrid: public BaseGrid
{
    
public:
    
    /**
     * @brief Constructor
     */
     OccupancyGrid();
     
     /**
      * @brief Destructor
      */
     ~OccupancyGrid();
     
    
};


//template and inline methods
inline
OccupancyGrid::~OccupancyGrid()
{}



    
    
    
    
}



#endif	/* OCCUPANCY_STRUCTURED_GRID_H */

