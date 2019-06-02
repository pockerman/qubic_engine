/* 
 * File:   grid_cell.h
 * Author: david
 *
 * Created on October 23, 2016, 1:55 PM
 */

#ifndef BASE_GRID_CELL_H
#define	BASE_GRID_CELL_H

#include "robosim/base/robosim.h"
//#include "robosim/base/indexed_object.h"
#include "robosim/graphics/polygon_renderer.h"
#include "robosim/base/robosim_config.h"

#include "utilities/indexed_object.h"

#ifdef ROBOSIM_DEBUG
#include "utilities/exceptions.h"
#include <string>
#endif

#include <vector>
#include <ostream>
#include <armadillo>



namespace robosim
{
  
//forward declarations
class BaseGrid;
    
/**
 * @brief Base class for all grid cells that the library supports
 */
    
class BaseGridCell: protected utilities::indexed_obj, public is_poly_rendered::renderer_type
{
    
public:
    
    using utilities::indexed_obj::get_id;
    using utilities::indexed_obj::set_id;
    
    /**
     * @brief Constructor
     */
    explicit BaseGridCell(size_type idx=RoboSimCommon::iuint(),const BaseGrid* grid = nullptr);
    
    /**
     * @brief Destructor
     */
    virtual ~BaseGridCell();
    
    /* @brief Return the occupation probability for
      * the i-th cell
      */
    real_type cell_probability()const{return oc_prob_;}
     
     /**
      * @brief Set the occupation probability for the i-th cell 
      */
    void set_cell_probability(real_type val){oc_prob_ = val;}
    
    /**
     * @brief Access to the neighbors list
     */
    std::vector<size_type>& get_neighbors(){return neighbors_;}
    
    
    /**
     * @brief Access to the neighbors list
     */
    const std::vector<size_type>& get_neighbors()const{return neighbors_;}
    
    
    /**
     * @brief Return the centroid of the cell
     */
    const arma::vec get_centroid()const;
    
    
    /**
     * @brief Return true if the occupation probability is
     * greater than the given tolerance
     */
    bool is_occupied(real_type tol = RoboSimCommon::tolerance())const;
    
    
    /**
     * @brief Set the number of neighbors the cell has
     */
    void set_n_neighbors(size_type nneighs);
    
    /**
     * @brief Set the i-th neighbor index
     */
    void set_neighbor_index(size_type i, size_type idx);
    
    
    /**
     * @brief Set the number of vertices the cell has 
     */
    void set_n_vertices(size_type nvertices);
    
    
    /**
     * @brief Set the v-th vertex index 
     */
    void set_vertex_index(size_type v, size_type idx);
    
    /**
     * @brief Print the cell information
     */
    std::ostream& print(std::ostream& out)const;
    
#ifdef ROBOSIM_HAS_OPENGL
    
    /**
     * @brief Render this cell. By default it assumes that
     * is dealing with a quad cell
     */
    void render();
#endif
    
protected:
    
    /**
     * @brief List of the neighbors of this cell
     */
    std::vector<size_type> neighbors_;
    
    
    /**
     * @brief List of the vertices of this cell
     */
    std::vector<size_type> vertices_;
    
    
    /**
     * @brief The occupancy probability for this cell
     */
    real_type oc_prob_;
    
    /**
     * @brief Pointer to the grid the cell belongs
     */
    const BaseGrid* grid_ptr_;
    
};

//template and inline methods

inline
BaseGridCell::~BaseGridCell()
{}

inline
void
BaseGridCell::set_n_neighbors(size_type nneighs){
    
    neighbors_.resize(nneighs,RoboSimCommon::iuint());
}

inline
void 
BaseGridCell::set_n_vertices(size_type nvertices){
   
    vertices_.resize(nvertices,RoboSimCommon::iuint());
}

inline
void 
BaseGridCell::set_neighbor_index(size_type i, size_type idx){
    
#ifdef ROBOSIM_DEBUG
    /*const std::string inmsg = "In BaseGridCell::set_neighbor_index(size_type, size_type).";
    using utilities::ExeLogicError;
    std::string msg = inmsg + " neighbors list not initialized";
    Assert(neighbors_.empty() == false,ExeLogicError(msg));*/
    utilities::assert_error(neighbors_.empty(),false,"In BaseGridCell::set_neighbor_index");
    utilities::assert_index_less_than_array_size(i,neighbors_.size(),"In BaseGridCell::set_neighbor_index");
    /*using utilities::ExeInvalidIndex;
    msg = inmsg+" Index i: "+
    std::to_string(i)+" not in [0,"+std::to_string(neighbors_.size())+")";
    Assert( i < neighbors_.size(),ExeInvalidIndex(msg));*/
#endif
    
    neighbors_[i] = idx;
}


inline
void 
BaseGridCell::set_vertex_index(size_type v, size_type idx){
    
/*#ifdef ROBOSIM_DEBUG
    const std::string inmsg = "In BaseGridCell::set_vertex_index(size_type, size_type).";
    using utilities::ExeLogicError;
    std::string msg = inmsg + " neighbors list not initialized";
    Assert(vertices_.empty() == false,ExeLogicError(msg));
    using utilities::ExeInvalidIndex;
    msg = inmsg+" Index v: "+
    std::to_string(v)+" not in [0,"+std::to_string(vertices_.size())+")";
    Assert( v < vertices_.size(),ExeInvalidIndex(msg));
#endif*/
    
    vertices_[v] = idx;
    
    
}


    
    
}



#endif	/* GRID_CELL_H */

