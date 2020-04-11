#ifndef BANG_BANG_PATH_TRACK_CONTROLER_H
#define BANG_BANG_PATH_TRACK_CONTROLER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/grids/waypoint_path.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/patterns/observer_base.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/discretization/utils/mesh_utils.h"
#include "kernel/geometry/shapes/circle.h"

#include "boost/noncopyable.hpp"
#include <cmath>
#include <tuple>

namespace cengine {
namespace control {

/// \brief Implements the path track controller
/// descibed in
/// A Smooth Path Tracking Algorithm for Wheeled
/// Mobile Robots with Dynamic Constraints
/// Journal of Intelligent and Robotic Systems
/// proposed by K. C. KOH and H. S. CHO
///

template<typename PointData, typename SegmentData>
class BangBangPathTrackController:public kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>,
                                  private boost::noncopyable
{
public:

    /// \brief The result returned by the execute
    struct Result
    {
        real_t v;
        real_t w;
    };

    typedef  Result result_t;

    /// \brief Expose the type of the path
    typedef typename kernel::ObserverBase<grids::WaypointPath<2,
                                                              PointData,
                                                              SegmentData>*>::resource_t  path_t;
    /// \brief Helper function
    static real_t clip(real_t a, real_t b);

    /// \brief Constructor
    BangBangPathTrackController();

    /// \brief Execute controller
    result_t execute(const kernel::dynamics::SysState<3>& state)const;

    /// \brief Set the lookahead distance
    void set_lookahead_dist(real_t dist){lookahead_distance_ = dist;}

    /// \brief Set the goal position
    void set_goal(const kernel::GeomPoint<2>& goal){goal_ = goal;}

    /// \brief Set the number of sampling points to
    /// use when computing the closest point from the
    /// position to the path
    void set_n_sampling_points(uint_t npoints){n_sampling_points_ = npoints; }

    /// \brief Set the maximum allowed acceleration
    void set_amax(real_t a){amax_ = a;}

    /// \brief Set the fitting coefficient
    void set_cx(real_t cx){cx_ = std::fabs(cx);}

    /// \brief Set the sampling time
    void set_dt(real_t dt){dt_ = dt;}

    /// \brief Update. Notify the observer that the
    /// resource is observing has been changed
    virtual void update(const path_t& resource) override final;

    /// \brief Read the resource
    virtual const path_t& read()const override final;

    /// \brief Calculate the linear velocity
    /// control
    real_t linear_velocity_control();

private:

    /// \brief The fitting coefficient
    real_t cx_;

    /// \brief Maximum acceleration
    real_t amax_;

    /// \brief The lookahead distance
    real_t lookahead_distance_;

    /// \brief number of sampling poinst to use
    /// when computing the closest point from
    /// position to the path
    uint_t n_sampling_points_;

    /// \brief The lookahed point calculated
    /// by the controller
    kernel::GeomPoint<2> lookahead_point_;

    /// \brief The goal coordinates
    kernel::GeomPoint<2> goal_;

    /// \brief The sampling time
    real_t dt_;

    /// \brief Tolerance used for calculations
    real_t tol_;
};

template<typename PointData, typename SegmentData>
real_t
BangBangPathTrackController<PointData, SegmentData>::clip(real_t a, real_t b){

    if(std::fabs(a) -b <kernel::KernelConsts::tolerance() ){
        return a;
    }

    return b;
}

template<typename PointData, typename SegmentData>
BangBangPathTrackController<PointData, SegmentData>::BangBangPathTrackController()
    :
    kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>(),
    cx_(0.0),
    amax_(0.0),
    lookahead_distance_(0.0),
    n_sampling_points_(kernel::KernelConsts::invalid_size_type()),
    lookahead_point_(),
    goal_(),
    dt_(0.0),
    tol_(kernel::KernelConsts::tolerance())
{}

template<typename PointData, typename SegmentData>
typename BangBangPathTrackController<PointData,SegmentData>::result_t
BangBangPathTrackController<PointData,
SegmentData>::execute(const kernel::dynamics::SysState<3>& state)const{

    /// calculate the linear velocity control
    ///

    /// get the position coordinates
    real_t rx = state.get("X");
    real_t ry = state.get("Y");

    /// form the position
    kernel::GeomPoint<2> position({rx, ry});

    const path_t& path=this->read();

    /// 2. Find the lookahead point. We can find the lookahead point
    /// by finding the intersection point of the circle centered at
    /// the robot's location and radius equal to the lookahead distance
    /// and the path segment
    auto intersections = kernel::discretization::utils::find_intersections(path,
                                                              kernel::Circle(lookahead_distance_, position));

    if(intersections.empty()){
        /// we cannot proceed
        throw std::logic_error("No intersection points found");
    }

    /// pick the first lookahead point
    /// as the target position on the path
    auto lookahead_point = intersections[0];

    /// get the segment that this lookahead point
    /// is on
    auto segment = kernel::discretization::utils::find_first_closest_element(path, lookahead_point, tol_);

}

template<typename PointData, typename SegmentData>
real_t
BangBangPathTrackController<PointData,SegmentData>::linear_velocity_control(){

    /// calculate vs
    real_t vs = 0.0;
    real_t ac = BangBangPathTrackController<PointData,SegmentData>::clip(vs/dt_, amax_);
    return ac*dt_;
}

template<typename PointData, typename SegmentData>
void
BangBangPathTrackController<PointData,
                            SegmentData>::update(const typename BangBangPathTrackController<PointData,
                                                                                            SegmentData>::path_t& resource){

    this->kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>::update(resource);
}

template<typename PointData, typename SegmentData>
const typename BangBangPathTrackController<PointData,SegmentData>::path_t&
BangBangPathTrackController<PointData,
                            SegmentData>::read()const{
  this->kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>::read();
}

}

}

#endif // BANG_BANG_PATH_TRACK_CONTROLER_H
