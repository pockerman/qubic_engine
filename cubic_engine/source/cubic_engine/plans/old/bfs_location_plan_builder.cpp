#include "robosim/plans/bfs_location_plan_builder.h"
#include "robosim/plans/location_plan.h"
#include "robosim/grids/occupancy_grid.h"
#include "robosim/grids/base_grid_cell.h"

#include "robosim/base/robosim_config.h"

#ifdef ROBOSIM_DEBUG
#include "utilities/exceptions.h"
#include <string>
#endif

#include <vector>
#include <utility>
#include <limits>
#include <queue>

namespace robosim
{
    
    
void 
BFSLocationPlanBuilder::build_shortest_path(const OccupancyGrid& grid,LocationPlan2D& plan,
                              size_type start,size_type goal,real_type tol){
    
    //clear all the points there may exist in the
    //plan
    plan.clear_locations();
    
    //quick return
    if(start==goal){
        
        const BaseGridCell* source = grid.cell(start);
        arma::vec center = source->get_centroid();
        plan.add_location_point(std::move(center));
        return;
    }


	//get the start cell
	const BaseGridCell* source = grid.cell(start);
        
        std::vector<bool> visited(grid.n_cells(),false);
        std::vector<size_type> path(grid.n_cells(),RoboSimCommon::iuint());
        
        visited[source->get_id()] = true;
       
	std::queue<const BaseGridCell*> queue; 
        queue.push(source);

        //dummy counter of the valid insertions performed
        size_type insert_counter = 0;
        
	//loop over the queue elements  
        //until the queue gets emptied

	while(!queue.empty()){

            const BaseGridCell* cell = queue.front();
            queue.pop();
                
            const std::vector<size_type>& neighbors = cell->get_neighbors();

            for(size_type n=0; n < neighbors.size(); ++n){
                    
                if(neighbors[n] != RoboSimCommon::iuint()){
                    const BaseGridCell* neigh = grid.cell(neighbors[n]);
                        
                    if(visited[neigh->get_id()] == false){
                            
                        //set the flag to true
                        visited[neigh->get_id()] = true;
                            
                        //if the cell is not occupied
                        if(!neigh->is_occupied(tol)){
                                path[neigh->get_id()] = cell->get_id();
                                insert_counter++;
                                queue.push(neigh);
                        }    
                    }
                }
            }

	}
/*#ifdef ROBOSIM_DEBUG
        //we must have a plan
        using utilities::ExeLogicError;
        const std::string msg="In BFSLocationPlanBuilder::build_shortest_path(). No valid path constructed";
        Assert(insert_counter != 0,ExeLogicError(msg));
#endif*/
        
        //let's construct the plan
        bool reached_start = false;
        std::vector<size_type> shortest_path;
        shortest_path.reserve(insert_counter);
        size_type came_from = path[goal];
        shortest_path.push_back(came_from);
        
        while(!reached_start){
            
            came_from = path[came_from];
            
            if(came_from == start)
                break;
            
            shortest_path.push_back(came_from);
        }
        
        plan.reserve_n_locations(insert_counter);
        
        const BaseGridCell* cell = grid.cell(start);
        arma::vec pos = cell->get_centroid();
        plan.add_location_point(pos);
        
        typedef std::vector<size_type>::reverse_iterator iterator;
        
        iterator rbegin = shortest_path.rbegin();
        iterator rend = shortest_path.rend();
        
        while(rbegin != rend){
            
            cell = grid.cell(*rbegin++);
            pos = cell->get_centroid();
            plan.add_location_point(pos);
        }
        
        //finally add the goal location
        cell = grid.cell(goal);
        pos = cell->get_centroid();
        plan.add_location_point(pos);
    }    
}
