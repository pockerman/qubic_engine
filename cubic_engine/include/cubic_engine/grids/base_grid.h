/* 
 * File:   base_grid.h
 * Author: david
 *
 * Created on October 29, 2016, 12:32 PM
 */

#ifndef BASE_GRID_H
#define	BASE_GRID_H

#include "robosim/base/robosim.h"
#include "robosim/graphics/renderer.h"
#include "robosim/base/robosim_config.h"

#ifdef ROBOSIM_DEBUG
#include "utilities/exceptions.h"
#include <string>
#endif





#include <vector>
#include <armadillo>

namespace robosim
{
    
//forward declarations
class BaseGridCell;
class CellVertex;
    
/**
 * @brief Base class for all grids the library supports
 */

class BaseGrid: public is_rendered::renderer_type
{
    
public:
    
    /**
     * @brief Constructor
     */
    BaseGrid();
    
     /**
      * @brief Destructor
      */
    virtual ~BaseGrid();
     
     
     /**
      * @brief Reserve space for the cells of the grid
      */
    void reserve_cells(size_type ncells){cells_.reserve(ncells);}
     
     
     
     /**
      * @brief Reserve space for the vertices of the grid
      */
    void reserve_vertices(size_type nvrts){vertices_.reserve(nvrts);}
    
    
     /**
      * @brief Returns the number of cells in the grid
      */
    size_type n_cells()const{return cells_.size();}
     
     
     /**
      * @brief Return the number of vertices 
      */
    size_type n_vertices()const{return vertices_.size();}
     
     /**
      * @brief Add a cell to the grid and get back the pointer
      */
    BaseGridCell* add_cell();
     
     
     /**
      * @brief Add a vertex to the grid
      */
    CellVertex* add_vertex();
     
     
     /**
      * @brief Add a vertex to the grid and get back the pointer
      */
    CellVertex* add_vertex(arma::vec&& centroid);
     
     /**
      * @brief Add a vertex to the grid and get back the pointer
      */
    CellVertex* add_vertex(std::initializer_list<real_type>&& centroid);
     
     
     /**
      * @brief Return the pointer to the i-th cell
      */
    BaseGridCell* cell(size_type i);
     
     
     /**
      * @brief Return the pointer to the i-th cell
      */
    const BaseGridCell* cell(size_type i)const;
    
    /**
     * @brief Return the v-th vertex of the grid 
     */
    CellVertex* vertex(size_type v);
    
    /**
     * @brief Return the v-th vertex of the grid 
     */
    const CellVertex* vertex(size_type v)const;
     
      /**
      * @brief Clears the memory allocated for the cells of this grid
      */
    void clear();
    
#ifdef ROBOSIM_HAS_OPENGL
    /**
     * @brief Render the grid 
     */
    virtual void render();
#endif
        
protected:
    
    /**
     * @brief The cells of the grid
     */
    std::vector<BaseGridCell*> cells_;
    
    
    /**
     * @brief The vertices of the grid
     */
    std::vector<CellVertex*> vertices_;
    
    

    
}; 

//template and inline methods
inline
BaseGrid::~BaseGrid(){
    
    clear();
}

inline
BaseGridCell* 
BaseGrid::cell(size_type i){
    return const_cast<BaseGridCell*>(static_cast<const BaseGrid&>(*this).cell(i));
}
  
inline
const BaseGridCell* 
BaseGrid::cell(size_type i)const{
/*#ifdef ROBOSIM_DEBUG
    using utilities::ExeLogicError;
    const std::string msg = " In BaseGrid::cell(size_type) invalid index: "+
    std::to_string(i)+" not in [0,"+std::to_string(cells_.size())+")";
    Assert(i < cells_.size(),ExeLogicError(msg));
#endif*/
    return cells_[i];
}

inline
CellVertex* 
BaseGrid::vertex(size_type v){
    
   return const_cast<CellVertex*>(static_cast<const BaseGrid&>(*this).vertex(v)); 
}
    
inline   
const CellVertex* 
BaseGrid::vertex(size_type v)const{
/*#ifdef ROBOSIM_DEBUG
    using utilities::ExeLogicError;
    const std::string msg = " In BaseGrid::vertex(size_type) invalid index: "+
    std::to_string(v)+" not in [0,"+std::to_string(vertices_.size())+")";
    Assert(v < vertices_.size(),ExeLogicError(msg));
#endif*/
    return vertices_[v];    
    
}
    
}



#endif	/* BASE_GRID_H */

