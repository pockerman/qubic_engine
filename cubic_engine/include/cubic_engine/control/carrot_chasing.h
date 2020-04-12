#ifndef CARROT_CHASING_H
#define CARROT_CHASING_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/grids/waypoint_path.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/patterns/observer_base.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/discretization/utils/mesh_utils.h"
#include "kernel/geometry/shapes/circle.h"
#include "kernel/utilities/common_uitls.h"

#include "boost/noncopyable.hpp"
#include <cmath>
#include <tuple>
#include <iostream>

namespace cengine {
namespace control {

/// \brief The so-called carrot chasing path track controller
///

template<typename PointData, typename SegmentData>
class CarrotChasingPathTrackController:public kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>,
                                  private boost::noncopyable
{
public:


    typedef  real_t result_t;

    /// \brief Expose the type of the path
    typedef typename kernel::ObserverBase<grids::WaypointPath<2,
                                                              PointData,
                                                              SegmentData>*>::resource_t  path_t;
    /// \brief Constructor
    CarrotChasingPathTrackController();

    /// \brief Execute controller
    result_t execute(const kernel::dynamics::SysState<3>& state)const;

    /// \brief Set the lookahead distance
    void set_lookahead_dist(real_t dist){lookahead_distance_ = dist;}

    /// \brief Set the number of sampling points to
    /// use when computing the closest point from the
    /// position to the path
    void set_n_sampling_points(uint_t npoints){n_sampling_points_ = npoints; }

    /// \brief Set the maximum allowed acceleration
    void set_gain(real_t k){k_ = k;}

    /// \brief Update. Notify the observer that the
    /// resource is observing has been changed
    virtual void update(const path_t& resource) override final;

    /// \brief Read the resource
    virtual const path_t& read()const override final;

private:

    /// \brief The gain parameter
    real_t k_;

    /// \brief The lookahead distance
    real_t lookahead_distance_;

    /// \brief number of sampling poinst to use
    /// when computing the closest point from
    /// position to the path
    uint_t n_sampling_points_;

    /// \brief Tolerance used for calculations
    real_t tol_;

};


template<typename PointData, typename SegmentData>
CarrotChasingPathTrackController<PointData, SegmentData>::CarrotChasingPathTrackController()
    :
    kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>(),
    k_(0.0),
    lookahead_distance_(0.0),
    n_sampling_points_(kernel::KernelConsts::invalid_size_type()),
    tol_(kernel::KernelConsts::tolerance())
{}

template<typename PointData, typename SegmentData>
typename CarrotChasingPathTrackController<PointData,SegmentData>::result_t
CarrotChasingPathTrackController<PointData,
SegmentData>::execute(const kernel::dynamics::SysState<3>& state)const{

    CarrotChasingPathTrackController<PointData,SegmentData>::result_t result;

    const kernel::GeomPoint<2> p({state.get("X"), state.get("Y")});
    const real_t theta = state.get("Theta");

    /// find out the segment at which
    /// we are closest to
    const path_t& path=this->read();

    auto intersections = kernel::discretization::utils::find_intersections(path,
                                                              kernel::Circle(lookahead_distance_, p));

    if(intersections.empty()){
        /// we cannot proceed
        throw std::logic_error("No intersection points found");
    }

    /// pick the first lookahead point
    /// as the target position on the path
    auto lookahead_point = intersections[0];

    auto segment = kernel::discretization::utils::find_closest_element(path, lookahead_point,
                                                                       n_sampling_points_, tol_);

    if(segment == nullptr){
        /// we cannot proceed
        throw std::logic_error("No segment found for lookahead point");
    }

    /*auto& start = segment->get_vertex(0);
    auto ru = start.distance(p);

    auto& end = segment->get_vertex(1);
    auto psi = std::atan2(end[1] - start[1], end[0] - start[0]);
    real_t psiu = std::atan2(p[1] - start[1], p[0] - start[0]);
    real_t beta = psi - psiu;

    real_t r = std::sqrt(kernel::utils::sqr(ru) - kernel::utils::sqr(ru*std::sin(beta)));*/
    //const kernel::GeomPoint<2> s({(r+lookahead_distance_)*std::cos(psi),
    //                             (r+lookahead_distance_)*std::sin(psi)});

    real_t theta_d =segment->get_orientation(); /*std::atan2((r+lookahead_distance_)*std::sin(psi) - p[1],
                                (r+lookahead_distance_)*std::cos(psi) - p[0]);*/

    return k_*(theta_d - theta);

}

template<typename PointData, typename SegmentData>
void
CarrotChasingPathTrackController<PointData,
                            SegmentData>::update(const typename CarrotChasingPathTrackController<PointData,
                                                             SegmentData>::path_t& resource){

    this->kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>::update(resource);
}

template<typename PointData, typename SegmentData>
const typename CarrotChasingPathTrackController<PointData, SegmentData>::path_t&
CarrotChasingPathTrackController<PointData, SegmentData>::read()const{
  this->kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>::read();
}

}

}

#endif // CARROT_CHASING_H
