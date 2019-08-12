/* 
 * File:   system_base.h
 * Author: david
 *
 * Created on November 9, 2016, 9:09 AM
 */

#ifndef SYSTEM_BASE_H
#define	SYSTEM_BASE_H

#include "cubic_engine/base/cubic_engine_types.h"

#include<boost/noncopyable.hpp>
#include <string>


namespace cengine
{
    
    
/**
 * @brief class for modelling  state-space systems
 */
    
template<typename Statetp, typename MotionTp, typename MeasurementTp, typename MatDescriptionTp>
class SystemBase: private boost::noncopyable
{
public:
    
    typedef Statetp state_type;
    typedef MotionTp motion_type;
    typedef MeasurementTp measurement_type;
    typedef MatDescriptionTp matrix_description_type;
    typedef typename  matrix_description_type::matrix_type matrix_type;
    
    /**
     * @brief Constructor
     */
    SystemBase(const std::string& name);
    
    /**
     * @brief Destructor 
     */
    virtual ~SystemBase();

    /**
     * @brief set_state Set the state of the of the system
     * @param state the state to set to
     */
    void set_state(const state_type& state){state_ = state;}

    /**
     * @brief set_state Set the state of the of the system. Uses move operation
     * @param state
     */
    void set_state(state_type&& state){state_ = std::move(state);}

    /**
     * @brief get_state Returns write reference to the state of the system
     */
    state_type& get_state(){return state_;}

    /**
     * @brief get_state Returns read reference to the state of the system
     */
    const state_type& get_state()const{return state_;}

    /**
     * @brief get_motion_model Returns read/write reference to the motion model
     */
    motion_type& get_motion_model(){return motion_model_;}

    /**
     * @brief get_motion_model Returns read reference to the motion model
     */
    const motion_type& get_motion_model()const{return motion_model_;}

    /**
     * @brief get_measurement_model. Returns read/write reference to the measurement model
     */
    measurement_type& get_measurement_model(){return meas_model_;}

    /**
     * @brief get_measurement_model. Returns read reference to the measurement model
     */
    const measurement_type& get_measurement_model()const{return meas_model_;}

    /**
     * @brief get_matrix. Returns read reference to the matrix_name matrix
     */
    const matrix_type& get_matrix(const std::string& matrix_name)const{return mat_descrp_.get_matrix(matrix_name);}

protected:

    /**
     * @brief name_ The name of the system
     */
    const std::string name_;

    /**
     * @brief state_ Describes the state of the system
     */
    state_type state_;

    /**
     * @brief motion_model_ Describes the motion model of the system
     */
    motion_type motion_model_;

    /**
     * @brief meas_model_ Describes the measurement model used by the system
     */
    measurement_type meas_model_;

    /**
     * @brief mat_descrp_ The matrix description of the system
     */
    matrix_description_type mat_descrp_;
};

template<typename Statetp, typename MotionTp, typename MeasurementTp, typename MatDescriptionTp>
SystemBase<Statetp, MotionTp, MeasurementTp, MatDescriptionTp>::SystemBase(const std::string& name)
    :
   name_(name),
   state_(),
   motion_model_(),
   meas_model_(),
   mat_descrp_()
{}

template<typename Statetp, typename MotionTp, typename MeasurementTp, typename MatDescriptionTp>
SystemBase<Statetp, MotionTp, MeasurementTp, MatDescriptionTp>:: ~SystemBase()
{}
    
}



#endif	/* SYSTEM_BASE_H */

