/* 
 * File:   cell_vertex.h
 * Author: david
 *
 * Created on October 25, 2016, 4:31 PM
 */

#ifndef CELL_VERTEX_H
#define	CELL_VERTEX_H

#include "robosim/base/robosim.h"
//#include "robosim/base/indexed_object.h"
#include "utilities/indexed_object.h"
#include <armadillo>

namespace robosim
{
    
/**
 * @brief Small class that represents a vertex
 * of a cell
 */
class CellVertex: protected utilities::indexed_obj
{
    
public:
    
    using utilities::indexed_obj::get_id;
    using utilities::indexed_obj::set_id;
    
    /**
     * @brief Constructor
     */
    explicit CellVertex(size_type idx=RoboSimCommon::iuint());
    
    
     /**
     * @brief Constructor
     */
    explicit CellVertex(const arma::vec& position,size_type idx=RoboSimCommon::iuint());
    
    /**
     * @brief Set the position
     */
    void set_position(const arma::vec& position){position_ = position;}
    
    /**
     * @brief Set the position
     */
    void set_position(const std::initializer_list<real_type>& pos){position_ = pos;}
    
    /**
     * @brief Return the position
     */
    arma::vec& get_position(){return position_;}
    
    /**
     * @brief Return the position
     */
    const arma::vec& get_position()const{return position_;}
    
    
    
private:
    
    
    
    /**
     * @brief The position of the vertex
     */
    arma::vec position_;
};
    
    
}



#endif	/* CELL_VERTEX_H */

