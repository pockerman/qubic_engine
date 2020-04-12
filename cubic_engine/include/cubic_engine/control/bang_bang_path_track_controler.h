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
#include "kernel/utilities/common_uitls.h"

#include "boost/noncopyable.hpp"
#include <cmath>
#include <tuple>
#include <iostream>

namespace cengine {
namespace control {

/// \brief Implements the path track controller
/// descibed in
/// A Smooth Path Tracking Algorithm for Wheeled
/// Mobile Robots with Dynamic Constraints
/// Journal of Intelligent and Robotic Systems
/// proposed by K. C. KOH and H. S. CHO
///

template<typename PointData, typename SegmentData, typename RefModel>
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
    result_t execute(const kernel::dynamics::SysState<3>& state,  real_t v)const;

    /// \brief Set the lookahead distance
    void set_lookahead_dist(real_t dist){lookahead_distance_ = dist;}

    /// \brief Set the number of sampling points to
    /// use when computing the closest point from the
    /// position to the path
    void set_n_sampling_points(uint_t npoints){n_sampling_points_ = npoints; }

    /// \brief Set the maximum allowed acceleration
    void set_amax(real_t a){amax_ = a;}

    /// \brief Set the maximum allowed angular acceleration
    void set_awmax(real_t awmax){awmax_ = awmax;}

    /// \brief Set the fitting coefficient
    void set_cx(real_t cx){cx_ = std::fabs(cx);}

    /// \brief Set the sampling time
    void set_dt(real_t dt){dt_ = dt;}

    /// \brief Update. Notify the observer that the
    /// resource is observing has been changed
    virtual void update(const path_t& resource) override final;

    /// \brief Read the resource
    virtual const path_t& read()const override final;

    /// \brief Returns the reference model
    RefModel& reference_model(){return ref_model_;}

private:

    /// \brief The fitting coefficient
    real_t cx_;

    /// \brief Maximum acceleration
    real_t amax_;

    /// \brief Maximum angular acceleration
    real_t awmax_;

    /// \brief The lookahead distance
    real_t lookahead_distance_;

    /// \brief number of sampling poinst to use
    /// when computing the closest point from
    /// position to the path
    uint_t n_sampling_points_;

    /// \brief The sampling time
    real_t dt_;

    /// \brief Tolerance used for calculations
    real_t tol_;

    /// \brief The reference model the controller
    /// integrates to
    mutable RefModel ref_model_;
};

template<typename PointData, typename SegmentData, typename RefModel>
real_t
BangBangPathTrackController<PointData, SegmentData, RefModel>::clip(real_t a, real_t b){

    if(std::fabs(a)<b ){
        return a;
    }

    return b;
}

template<typename PointData, typename SegmentData, typename RefModel>
BangBangPathTrackController<PointData, SegmentData, RefModel>::BangBangPathTrackController()
    :
    kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>(),
    cx_(0.0),
    amax_(0.0),
    awmax_(0.0),
    lookahead_distance_(0.0),
    n_sampling_points_(kernel::KernelConsts::invalid_size_type()),
    dt_(0.0),
    tol_(kernel::KernelConsts::tolerance()),
    ref_model_()
{}

template<typename PointData, typename SegmentData, typename RefModel>
typename BangBangPathTrackController<PointData,SegmentData,RefModel>::result_t
BangBangPathTrackController<PointData,
SegmentData,RefModel>::execute(const kernel::dynamics::SysState<3>& state, real_t v)const{

    BangBangPathTrackController<PointData,SegmentData, RefModel>::result_t result;

    ref_model_.set_time_step(dt_);

    /// get the position coordinates
    real_t rx = state.get("X");
    real_t ry = state.get("Y");

    /// form the position
    kernel::GeomPoint<2> position({rx, ry});

    /// get the position of the reference model
    real_t ref_x = ref_model_.get_x_position();
    real_t ref_y = ref_model_.get_y_position();

    /// form the position
    kernel::GeomPoint<2> ref_position({ref_x, ref_y});

    /// calculate the vector
    kernel::GeomPoint<2> vec = ref_position - position;

    /// the path to follow
    const path_t& path=this->read();

    /// Find the lookahead point. We can find the lookahead point
    /// by finding the intersection point of the circle centered at
    /// the robot's location and radius equal to the lookahead distance
    /// and the path segment
    /*auto intersections = kernel::discretization::utils::find_intersections(path,
                                                              kernel::Circle(lookahead_distance_, position));

    if(intersections.empty()){
        /// we cannot proceed
        throw std::logic_error("No intersection points found");
    }*/

    /// pick the first lookahead point
    /// as the target position on the path
    auto lookahead_point = ref_position; //intersections[0];
    //auto lookahead_point = intersections[0];

    /// get the segment that this lookahead point
    /// is on
    auto segment = kernel::discretization::utils::find_closest_element(path, ref_position, /*lookahead_point,*/
                                                                       n_sampling_points_, tol_);

    if(segment == nullptr){
        /// we cannot proceed
        throw std::logic_error("No segment found for lookahead point");
    }

    auto start = segment->get_vertex(0);
    auto end = segment->get_vertex(1);

    auto seg_vec = end - start;

    real_t theta_t = ref_model_.get_orientation(); //segment->get_orientation();
    real_t wt = segment->get_angular_velocity();
    real_t vt = segment->get_velocity();

    /// calculate errors
    real_t ex = (lookahead_point[0] - rx)*std::cos(theta_t) +
                (lookahead_point[1] - ry)*std::sin(theta_t);

    real_t ey = (lookahead_point[0] - rx)*std::sin(theta_t) -
                (lookahead_point[1] - ry)*std::cos(theta_t);

    real_t etheta = theta_t - state.get("Theta");

    //std::cout<<"ex: "<<ex<<std::endl;
    //std::cout<<"ey: "<<ey<<std::endl;
    //std::cout<<"etheta: "<<etheta<<std::endl;

    real_t dotey = v*std::sin(etheta) - wt*ex ;
    real_t dotex = vt - v*std::cos(etheta) + wt*ey;

    ///calculate landing point
    /// orientation and angular velocity
    ///

    real_t atan = std::atan(3.0*cx_* std::pow(ey/cx_, 2/3)*kernel::utils::sign(ey));
    if(std::fabs(ey)<tol_){
        atan = 0.0;
    }


    real_t theta_p = theta_t + atan;

    real_t tan = std::tan(atan);
    real_t tansqr = kernel::utils::sqr(tan);
    real_t wp = wt + (2.0*std::pow(ey/cx_, -1/3)/(1 + tansqr))*dotey*kernel::utils::sign(ey);

    int sign = kernel::utils::sign(theta_p - state.get("Theta"));

    std::cout<<"Sign is : "<<sign<<std::endl;
    real_t ws = wp +
            std::sqrt(2.0*awmax_*std::fabs(theta_p - state.get("Theta")))*sign;


    real_t awc = BangBangPathTrackController<PointData, SegmentData, RefModel>::clip(ws/dt_, awmax_);

    result.w = awc*dt_;

    real_t vs = dotex + std::sqrt(2.0*amax_*std::fabs(ex))*kernel::utils::sign(ex);
    real_t ac = BangBangPathTrackController<PointData, SegmentData, RefModel>::clip(vs/dt_, amax_);

    result.v = ac*dt_;

    /// integrate the reference model
    ref_model_.integrate(vt, wt);

    //std::cout<<"Reference model state: "<<std::endl;
    //std::cout<<ref_model_.get_state()<<std::endl;

    return result;

}

template<typename PointData, typename SegmentData, typename RefModel>
void
BangBangPathTrackController<PointData,
                            SegmentData,
RefModel>::update(const typename BangBangPathTrackController<PointData,
                                                             SegmentData,
                  RefModel>::path_t& resource){

    this->kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>::update(resource);
}

template<typename PointData, typename SegmentData, typename RefModel>
const typename BangBangPathTrackController<PointData, SegmentData, RefModel>::path_t&
BangBangPathTrackController<PointData,
                            SegmentData, RefModel>::read()const{
  this->kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>::read();
}

}

}

#endif // BANG_BANG_PATH_TRACK_CONTROLER_H
