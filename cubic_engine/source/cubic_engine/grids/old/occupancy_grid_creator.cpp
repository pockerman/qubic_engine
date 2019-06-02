#include "robosim/grids/occupancy_grid_creator.h"
#include "robosim/grids/occupancy_grid.h"
#include "robosim/grids/base_grid_cell.h"

#include "robosim/base/robosim_config.h"

#ifdef ROBOSIM_DEBUG
#include "utilities/exceptions.h"
#include <string>
#include <c++/5/bits/basic_string.h>
#endif

namespace robosim
{
    
    
    namespace
    {
      
      //return the gloabl vertex id for quad cells
      size_type vertex_idx(size_type nx,size_type i,size_type j)
      {
	
        return i+j*(nx+1);    
      }
      
      /**
       * @brief Build the vertices for the grid
       */
      void build_grid_vertices(const arma::vec& low, const arma::vec& high,
                               size_type Nx,size_type Ny,OccupancyGrid& grid){
          
        //reserve the number of vertices for the grid
        grid.reserve_vertices((Nx+1)*(Ny+1));
        real_type dx = (high(0) - low(0))/Nx;
        real_type dy = (high(1) - low(1))/Ny;
          
        //create the vertices
        for(size_type y=0; y < (Ny+1); ++y){
        
            real_type ycoord = low(1) + y*dy;
        
            for(size_type x=0; x < (Nx+1); ++x){
            
                real_type xcoord = low(0) + x*dx;
           
                grid.add_vertex({xcoord,ycoord});
            }
        }   
      }
      
      /**
       * @brief Build a grid that only has elements in the x-direction
       */
      void build_1d_x(size_type Nx, OccupancyGrid& grid){
            
        size_type gid = 0;
    
        for(size_type nx=0; nx<Nx; ++nx){
            
            BaseGridCell* cell = grid.add_cell();
            
            //set the number of neighbors
            cell->set_n_neighbors(4);
        
            //set the number of vertices
            cell->set_n_vertices(4);
            
            cell->set_vertex_index(0,vertex_idx(Nx,nx,0));
            cell->set_vertex_index(1,vertex_idx(Nx,nx+1,0));
            cell->set_vertex_index(2,vertex_idx(Nx,nx+1,1));
            cell->set_vertex_index(3,vertex_idx(Nx,nx,1));
            
            //first bottom row
                
            if(gid == 0){
               cell->set_neighbor_index(1,gid+1);        
            }
            else{
                 if(nx != (Nx-1))
                    cell->set_neighbor_index(1,gid+1);
                    
               cell->set_neighbor_index(3,gid-1);         
            }  
            gid++;
        }
      }
    }

    
void
OccupancyGridCreator::build(const arma::vec& low, const arma::vec& high, 
                            size_type Nx, size_type Ny, 
                            OccupancyGrid& grid){
    

/*#ifdef ROBOSIM_DEBUG
    using utilities::ExeLogicError;
    std::string msg="Invalid number of elements Nx and Ny should not be zero";
    Assert(((Nx != 0) && (Ny != 0)),ExeLogicError(msg));
#endif*/
    
    const size_type nelems = Nx*Ny;
    
    //reserve the number of cells for the grid
    grid.reserve_cells(nelems);
    
    //reserve the number of vertices for the grid
    //grid.reserve_vertices((Nx+1)*(Ny+1));
    
    build_grid_vertices(low,high,Nx,Ny,grid);
    
    /*real_type dx = (high(0) - low(0))/Nx;
    real_type dy = (high(1) - low(1))/Ny;
    
    //create the vertices
    for(size_type y=0; y < (Ny+1); ++y){
        
        real_type ycoord = low(1) + y*dy;
        
        for(size_type x=0; x < (Nx+1); ++x){
            
           real_type xcoord = low(0) + x*dx;
           
           grid.add_vertex({xcoord,ycoord});
        }
    }*/
    
    if(Ny==1){
        
        build_1d_x(Nx,grid);
        return;
    }
    
    size_type gid = 0;
    
    //create the elements and the  neighbors
    for(size_type ny=0; ny<Ny; ++ny){
        
        for(size_type nx=0; nx<Nx; ++nx){
            
            BaseGridCell* cell = grid.add_cell();
            
            //set the number of neighbors
            cell->set_n_neighbors(4);
        
            //set the number of vertices
            cell->set_n_vertices(4);
            
            cell->set_vertex_index(0,vertex_idx(Nx,nx,ny));
            cell->set_vertex_index(1,vertex_idx(Nx,nx+1,ny));
            cell->set_vertex_index(2,vertex_idx(Nx,nx+1,ny+1));
            cell->set_vertex_index(3,vertex_idx(Nx,nx,ny+1));
            
            //BaseGridCell* cell = grid.cell(gid); 
            
            if(ny == 0){ //first bottom row
                
                if(gid == 0){
                
                    cell->set_neighbor_index(1,gid+1);
                    cell->set_neighbor_index(2,Nx);
                }
                else{
                    if(nx != (Nx-1))
                        cell->set_neighbor_index(1,gid+1);
                    
                    cell->set_neighbor_index(3,gid-1);
                    cell->set_neighbor_index(2,gid+Nx);     
                }  
            }//nx==0
            else if((ny != 0 && ny != (Ny-1))){ //middle rows except the top row
                
               if(nx == 0){
                
                    cell->set_neighbor_index(0,gid - Nx);
                    cell->set_neighbor_index(1,gid + 1);
                    cell->set_neighbor_index(2,gid + Nx);
                }
                else{
                   
                    cell->set_neighbor_index(0,gid - Nx);
                    
                    if(nx != (Nx-1))
                        cell->set_neighbor_index(1,gid + 1);
                    
                    cell->set_neighbor_index(2,gid + Nx);
                    cell->set_neighbor_index(3,gid-1); 
                }   
            }
            else if(ny == (Ny-1)){ //top row
                
                if(nx == 0){
                
                    cell->set_neighbor_index(0,gid - Nx);
                    cell->set_neighbor_index(1,gid + 1);
                }
                else{
                   
                    cell->set_neighbor_index(0,gid - Nx);
                    
                    if(nx != (Nx-1))
                        cell->set_neighbor_index(1,gid + 1);
                    
                    cell->set_neighbor_index(3,gid-1); 
                }      
            }
            
            gid++;
        }
        
    }
    
    
}

void 
OccupancyGridCreator::build(const arma::vec& low, const arma::vec& high,
                            size_type Nx,size_type Ny,std::vector<real_type>& probs,
                            OccupancyGrid& grid){
    
    OccupancyGridCreator::build(low,high,Nx,Ny,grid);
    
/*#ifdef ROBOSIM_DEBUG
    using utilities::ExeLogicError;
    const std::string msg="In OccupancyGridCreator::build. Probabilities vector size: "+
    std::to_string(probs.size())+" not equal to number of cells: "+std::to_string(grid.n_cells());
    Assert(probs.size() == grid.n_cells(),ExeLogicError(msg));
#endif*/
    
    for(size_type c=0; c<grid.n_cells(); ++c){
      //if we are given occupation propbabilities
      //set them
      BaseGridCell* cell = grid.cell(c);
      cell->set_cell_probability(probs[cell->get_id()]);        
    }
}


}
