#ifndef VEHICLE_BASE_H
#define VEHICLE_BASE_H

#include "kernel/base/types.h"

namespace kernel{
namespace vehicles{

///
/// \brief The VehicleBase class. Base class for
/// modeling planar vehicles
///
template<typename DynamicsTp>
class VehicleBase
{

public:

    ///
    /// \brief dynamics_t. The type of the object describing
    /// the dynamics
    ///
    typedef DynamicsTp dynamics_t;

    ///
    /// \brief state_t. The type of the object describing the
    /// state
    ///
    typedef typename DynamicsTp::state_t state_t;

    ///
    /// \brief VehicleBase. Constructor
    ///
    VehicleBase()=default;

    ///
    /// \brief VehicleBase. Constructor
    ///
    template<typename DynamicsInTp>
    VehicleBase(const DynamicsInTp& dynamics_in);

    ///
    /// \brief Read the x-coordinate
    ///
    real_t get_x_position()const{return dynamics_.get_x_position(); }

    ///
    /// \brief Set the x-coordinate
    ///
    void set_x_position(real_t x){dynamics_.set_x_position(x);}

    ///
    /// \brief Read the y-coordinate
    ///
    real_t get_y_position()const{return dynamics_.get_y_position();}

    ///
    /// \brief Set the y-coordinate
    ///
    void set_y_position(real_t y){dynamics_.set_y_position(y);}

    ///
    /// \brief Write the position to the given
    /// type. Type must support operator[]
    ///
    template<typename Type>
    void get_position(Type& pos)const;

    ///
    /// \brief Read the orientation
    ///
    real_t get_orientation()const{return dynamics_.get_orientation();}

    ///
    /// \brief Set the orientation
    ///
    void set_orientation(real_t theta){dynamics_.set_orientation(theta);}

    ///
    /// \brief Set time step
    ///
    void set_time_step(real_t dt){dynamics_.set_time_step(dt);}

    ///
    /// \brief Returns the state of the vehicle
    ///
    const state_t& get_state()const{return dynamics_.get_state();}

protected:

    ///
    /// \brief The object that handles the dynamics
    ///
    dynamics_t dynamics_;

};


template<typename DynamicsTp>
template<typename DynamicsInTp>
VehicleBase<DynamicsTp>::VehicleBase(const DynamicsInTp& dynamics_in)
    :
    dynamics_(dynamics_in)
{}

template<typename DynamicsTp>
template<typename Type>
void
VehicleBase<DynamicsTp>::get_position(Type& pos)const{

    pos[0] = get_x_position();
    pos[1] = get_y_position();
}
}
}

#endif // VEHICLE_BASE_H
