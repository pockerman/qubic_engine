#ifndef CARROT_CHASING_H
#define CARROT_CHASING_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/grids/waypoint_path.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/patterns/observer_base.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/geometry/geometry_utils.h"
#include "kernel/geometry/shapes/circle.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/base/unit_converter.h"

#include "boost/noncopyable.hpp"
#include <cmath>
#include <tuple>
#include <iostream>

namespace cengine {
namespace control {

/// \brief The so-called carrot chasing path track controller
template<typename PointData, typename SegmentData>
class CarrotChasingPathTrackController:public kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>,
                                  private boost::noncopyable
{
public:


    /// \brief The result returned by the controller
    /// upon calling execute
    typedef  real_t result_t;

    /// \brief Expose the type of the path
    typedef typename kernel::ObserverBase<grids::WaypointPath<2,
                                                              PointData,
                                                              SegmentData>*>::resource_t  path_t;
    /// \brief Constructor
    CarrotChasingPathTrackController();

    /// \brief Execute controller
    std::tuple<result_t, kernel::GeomPoint<2>, kernel::GeomPoint<2>> execute(const kernel::dynamics::SysState<3>& state)const;

    /// \brief Set the lookahead distance
    void set_lookahead_dist(real_t dist){lookahead_distance_ = dist;}

    /// \brief Set the number of sampling points to
    /// use when computing the closest point from the
    /// position to the path
    void set_n_sampling_points(uint_t npoints){n_sampling_points_ = npoints; }

    /// \brief Set the maximum allowed acceleration
    void set_gain(real_t k){k_ = k;}

    /// \brief Set the waypoint radius
    void set_wpoint_radius(real_t r){waypoint_r_ = r;}

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

    /// \brief Radius within which it is
    /// assumed that the waypoint has been reached
    real_t waypoint_r_;

    typedef typename path_t::element_t element_t;

    /// \brief The current segment the algorithm is using
    mutable const element_t* current_element_;

};


template<typename PointData, typename SegmentData>
CarrotChasingPathTrackController<PointData, SegmentData>::CarrotChasingPathTrackController()
    :
    kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>(),
    k_(0.0),
    lookahead_distance_(0.0),
    n_sampling_points_(kernel::KernelConsts::invalid_size_type()),
    tol_(kernel::KernelConsts::tolerance()),
    waypoint_r_(kernel::KernelConsts::tolerance()),
    current_element_(nullptr)
{}

template<typename PointData, typename SegmentData>
std::tuple<typename CarrotChasingPathTrackController<PointData,SegmentData>::result_t,
kernel::GeomPoint<2>, kernel::GeomPoint<2>>
CarrotChasingPathTrackController<PointData,
SegmentData>::execute(const kernel::dynamics::SysState<3>& state)const{

    CarrotChasingPathTrackController<PointData,SegmentData>::result_t result;

    const kernel::GeomPoint<2> p({state.get("X"), state.get("Y")});
    const real_t theta = state.get("Theta");

    /// find out the segment at which
    /// we are closest to
    const path_t& path=this->read();

    auto [closest_path_point, segment] = kernel::find_closest_point_to(path, p, n_sampling_points_, tol_);

    if(segment == nullptr){
         /// we cannot proceed
         throw std::logic_error("No segment found for lookahead point");
     }

    current_element_ = segment;

    std::cout<<"Segment id found: "<<current_element_->get_id()<<std::endl;

    /// find the look ahead point
    auto [found, lookahead_point] = kernel::find_point_on_line_distant_from_p(*current_element_,
                                                                              closest_path_point,
                                                                              lookahead_distance_,
                                                                               n_sampling_points_,
                                                                               tol_);

    if(!found){
       /// the lookahed point is the next waypoint
       lookahead_point = current_element_->get_vertex(1);
    }


    real_t theta_d = std::atan2(lookahead_point[1] - p[1], lookahead_point[0] - p[0]);
    return {k_*(theta_d - theta), lookahead_point, closest_path_point};


    /// maybe the robot reached the segment end
    /// if yes then we proceed to use the next segment
    /*if(p.distance(current_element_->get_vertex(1)) < waypoint_r_){

        if(current_element_->get_id() + 1 != path.n_elements()){
            /// change the segment we are looking at
            current_element_ = path.element(current_element_->get_id() +1);
        }
        else{
            throw std::logic_error("End of path reached");
        }

        std::cout<<"Segment id used: "<<current_element_->get_id()<<std::endl;

        auto& start = current_element_->get_vertex(0);
        auto ru = start.distance(p);

        auto& end = current_element_->get_vertex(1);



        auto psi = std::atan2(end[1] - start[1], end[0] - start[0]);
        std::cout<<"Segment angle: "<<psi<<std::endl;
        real_t psiu = std::atan2(p[1] - start[1], p[0] - start[0]);
        real_t beta = psi - psiu;

        real_t r = std::sqrt(kernel::utils::sqr(ru) - kernel::utils::sqr(ru*std::sin(beta)));
        real_t theta_d =std::atan2((r+lookahead_distance_)*std::sin(psi) - p[1],
                                    (r+lookahead_distance_)*std::cos(psi) - p[0]);

        //if(std::fabs(theta_d) > kernel::UnitConverter::degrees_to_rad(90.0) ){
        //    theta_d = current_element_->get_orientation();
        //}

        //real_t theta_d = current_element_->get_orientation();
        std::cout<<"Desired angle: "<<kernel::UnitConverter::rad_to_degrees(theta_d)<<std::endl;
        std::cout<<"Error angle: "<<kernel::UnitConverter::rad_to_degrees(theta_d - theta)<<std::endl;
        std::cout<<"R: "<<r<<", delta: "<<lookahead_distance_<<std::endl;
        std::cout<<"Lookahed point: "<<(r+lookahead_distance_)*std::cos(psi)<<", "
                                     <<(r+lookahead_distance_)*std::sin(psi)<<std::endl;




        /// 2. Find the lookahead point. We can find the lookahead point
        /// by finding the intersection point of the circle centered at
        /// the robot's location and radius equal to the lookahead distance
        /// and the path segment
        auto intersections = kernel::discretization::utils::find_intersections(path,
                                                                  kernel::Circle(lookahead_distance_, p));

        if(intersections.empty()){
            /// we cannot proceed
            throw std::logic_error("No intersection points found");
        }

        std::cout<<"Lookahed point: "<<intersections[0][0]<<", "
                                     <<intersections[0][1]<<std::endl;

        theta_d = std::atan2(intersections[0][1] - p[1], intersections[0][0] - p[0]);
        return {k_*(theta_d - theta), intersections[0]};
    }
    else{

        auto closest_path_point = kernel::discretization::utils::find_point_on_element_closest_to(*current_element_,
                                                                                                  p,
                                                                                                  n_sampling_points_,
                                                                                                  tol_);
        /// we have not reached the segment end
        auto dist = closest_path_point.distance(current_element_->get_vertex(1));

        auto lookahead_distance = lookahead_distance_;

        if( dist < lookahead_distance_){
            lookahead_distance = dist;
        }

        std::cout<<"Segment id used: "<<current_element_->get_id()<<std::endl;

        auto& start = current_element_->get_vertex(0);
        auto ru = start.distance(p);

        auto& end = current_element_->get_vertex(1);
        auto psi = std::atan2(end[1] - start[1], end[0] - start[0]);
        std::cout<<"Segment angle: "<<psi<<std::endl;
        real_t psiu = std::atan2(p[1] - start[1], p[0] - start[0]);
        real_t beta = psi - psiu;

        real_t r = std::sqrt(kernel::utils::sqr(ru) - kernel::utils::sqr(ru*std::sin(beta)));
        real_t theta_d =std::atan2((r+lookahead_distance)*std::sin(psi) - p[1],
                                    (r+lookahead_distance)*std::cos(psi) - p[0]);

        //if(std::fabs(theta_d) > kernel::UnitConverter::degrees_to_rad(90.0) ){
        //    theta_d = current_element_->get_orientation();
        //}

        //real_t theta_d = current_element_->get_orientation();

        std::cout<<"Desired angle: "<<kernel::UnitConverter::rad_to_degrees(theta_d)<<std::endl;
        std::cout<<"Error angle: "<<kernel::UnitConverter::rad_to_degrees(theta_d - theta)<<std::endl;
        std::cout<<"R: "<<r<<", delta: "<<lookahead_distance_<<std::endl;
        std::cout<<"Lookahed point: "<<(r+lookahead_distance_)*std::cos(psi)<<", "
                                     <<(r+lookahead_distance_)*std::sin(psi)<<std::endl;

        /// 2. Find the lookahead point. We can find the lookahead point
        /// by finding the intersection point of the circle centered at
        /// the robot's location and radius equal to the lookahead distance
        /// and the path segment
        auto intersections = kernel::discretization::utils::find_intersections(path,
                                                                  kernel::Circle(lookahead_distance_, p));

        if(intersections.empty()){
            /// we cannot proceed
            throw std::logic_error("No intersection points found");
        }

        std::cout<<"Lookahed point: "<<intersections[0][0]<<", "
                                     <<intersections[0][1]<<std::endl;

        theta_d = std::atan2(intersections[0][1] - p[1], intersections[0][0] - p[0]);
        return {k_*(theta_d - theta), intersections[0]};
    }*/

    /// if the distance from the closest point
    /// to the end of the segment is less
    /// than the lookahead distance then the lookahead
    ///
    ///  we will switch
    /// segments and consider the next segment.
    /// We do this if the found segment is not
    /// the last segment on the path

    /*if(segment->get_id() + 1 != path.n_elements()){

        if(closest_path_point.distance(segment->get_vertex(1)) < lookahead_distance_){

            /// change the segment we are looking at
            segment = path.element(segment->get_id() +1);
        }


    }
    else{



    }*/




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
