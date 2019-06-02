/* 
 * File:   location_plan_builder.h
 * Author: david
 *
 * Created on October 24, 2016, 5:54 PM
 */

#ifndef BFS_LOCATION_PLAN_BUILDER_H
#define	BFS_LOCATION_PLAN_BUILDER_H

#include "robosim/base/robosim.h"

namespace robosim
{
    
//forward declarations
class LocationPlan2D;
class OccupancyGrid;
    
/**
 * @brief Class for creating location plans.
 * This class uses BFS (breadth first search) to construct
 * the location path
 */

class BFSLocationPlanBuilder
{
    
public:
    
    /**
     * @brief Given the OccupancyGrid and the start and goal cells ids build the 2D location plan
     */
    static void build_shortest_path(const OccupancyGrid& grid,LocationPlan2D& plan,
                      size_type start,size_type goal,real_type tol=RoboSimCommon::tolerance());
    
    
};    
    
}



#endif	/* LOCATION_PLAN_BUILDER_H */

