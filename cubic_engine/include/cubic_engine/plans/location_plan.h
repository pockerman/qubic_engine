/* 
 * File:   location_plan.h
 * Author: david
 *
 * Created on October 23, 2016, 1:41 PM
 */

#ifndef LOCATION_PLAN_H
#define	LOCATION_PLAN_H

#include "robosim/base/robosim.h"
#include "robosim/base/robosim_config.h"

#ifdef ROBOSIM_DEBUG
#include "utilities/exceptions.h"
#include <string>
#endif

#include <vector>
#include <armadillo>

namespace robosim
{
    
/**
 * @brief Class that describes a plan based on 2D locations.
 * This class accepts a start 2D location and a goal 2D location.
 * Given an assembly of location landmarks and a solver it
 * solves for the path planning which needs to be followed
 * in order to reach the goal location
 */
class LocationPlan2D
{
    
public:
    
    /**
     * @brief Constructor
     */
    LocationPlan2D();
    
    /**
     * @brief Constructor
     */
    LocationPlan2D(const arma::vec& start,const arma::vec& goal);
    
    
    /**
     * @brief Set the start location of the plan
     */
    void set_start_location(const arma::vec& s){start_ = s;}
    
    /**
     * @brief Return the start location of the plan
     */
    arma::vec& get_start(){return start_;}
    
    /**
     * @brief Return the start location of the plan
     */
    const arma::vec& get_start()const{return start_;}
    
    /**
     * @brief Set the start location of the plan
     */
    void set_goal_location(const arma::vec& g){goal_ = g;}
    
    /**
     * @brief Return the start location of the plan
     */
    arma::vec& get_goal(){return goal_;}
    
    /**
     * @brief Return the start location of the plan
     */
    const arma::vec& get_goal()const{return goal_;}
    
    /**
     * @brief Return the locations of the plan
     */
    std::vector<arma::vec>& get_locations(){return locations_;}
    
    /**
     * @brief Return the locations of the plan
     */
    const std::vector<arma::vec>& get_locations()const{return locations_;}
    
    /**
     * @brief Return the location of the landmark with the given id
     */
    arma::vec& get_landmark_location(size_type id);
    
    /**
     * @brief Return the location of the landmark with the given id
     */
    const arma::vec& get_landmark_location(size_type id)const;
    
    
    /**
     * @brief Clears the locations of this plan
     */
    void clear_locations(){locations_.clear();}
    
    
    /**
     * @brief Push a location point in the plan
     */
    void add_location_point(arma::vec&& point);
    
    
    /**
     * @breif Push a location point in the plan
     */
    void add_location_point(const arma::vec& point);
    
    
    /**
     * @brief Reserve space for the locations
     */
    void reserve_n_locations(size_type n){locations_.reserve(n);}
    
    
    /**
     * @brief Save in CSV format the locations of this plan
     */
    void save(const std::string& filename);
    
    
private:
    
    /**
     * @brief The start location
     */
    arma::vec start_;
    
    /**
     * @brief The goal location
     */
    arma::vec goal_;
    
    /**
     * @brief The series of locations this plan has.
     */
    std::vector<arma::vec> locations_;
    
};

//inline and template methods
inline
void 
LocationPlan2D::add_location_point(arma::vec&& point){
    
    locations_.push_back(std::move(point));
    point.clear();

} 

inline
void 
LocationPlan2D::add_location_point(const arma::vec& point){
    
   locations_.push_back(point); 
}

inline
arma::vec& 
LocationPlan2D::get_landmark_location(size_type id){
    
    return const_cast<arma::vec&>(static_cast<const LocationPlan2D&>(*this).get_landmark_location(id) );
}
    
inline   
const arma::vec& 
LocationPlan2D::get_landmark_location(size_type id)const{
    
/*#ifdef ROBOSIM_DEBUG
    using utilities::ExeLogicError;
    const std::string msg="In LocationPlan2D::get_landmark_location(size_type)const. Index: "+
    std::to_string(id)+" not in [0,"+std::to_string(locations_.size())+")";
    Assert(id < locations_.size(),ExeLogicError(msg));
#endif*/
    
    return locations_[id];
    
}


    
    
    
}



#endif	/* LOCATION_PLAN_H */

