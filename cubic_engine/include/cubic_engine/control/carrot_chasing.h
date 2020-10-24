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

///
/// \brief Helper class to assemble all the
/// properties needed to initialize the
/// CarrotChasingPathTrackController
///
struct CarrotChasingPathTrackControllerInput
{

    ///
    /// \brief The gain parameter
    ///
    real_t k;

    ///
    /// \brief The lookahead distance
    ///
    real_t lookahead_distance;

    ///
    /// \brief number of sampling poinst to use
    /// when computing the closest point from
    /// position to the path
    ///
    uint_t n_sampling_points;

    ///
    /// \brief Tolerance used for calculations
    ///
    real_t tol;

    ///
    /// \brief Radius within which it is
    /// assumed that the waypoint has been reached
    ///
    real_t waypoint_r;
};

///
/// \brief The class CarrotChasingPathTrackController.
/// Implements the so-called carrot chasing path track controller
///
template<typename PointData, typename SegmentData>
class CarrotChasingPathTrackController:public kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>,
                                  private boost::noncopyable
{
public:

    ///
    /// \brief The result returned by the controller
    /// upon calling execute
    ///
    typedef  std::tuple<real_t, kernel::GeomPoint<2>, kernel::GeomPoint<2>>  result_t;

    ///
    /// \brief The type of the base class
    ///
    typedef kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*> obsertver_base_t;

    /// \brief Expose the type of the path
    typedef typename kernel::ObserverBase<grids::WaypointPath<2,
                                                              PointData,
                                                              SegmentData>*>::resource_t  path_t;
    ///
    /// \brief Constructor
    ///
    CarrotChasingPathTrackController();

    ///
    /// \brief Constructor
    ///
    CarrotChasingPathTrackController(const CarrotChasingPathTrackControllerInput& input );

    ///
    /// \brief Execute controller
    ///
    std::tuple<real_t, kernel::GeomPoint<2>, kernel::GeomPoint<2>>
    execute(const kernel::dynamics::SysState<3>& state)const;

    ///
    /// \brief Set the lookahead distance
    ///
    void set_lookahead_dist(real_t dist){lookahead_distance_ = dist;}

    ///
    /// \brief Set the number of sampling points to
    /// use when computing the closest point from the
    /// position to the path
    ///
    void set_n_sampling_points(uint_t npoints){n_sampling_points_ = npoints; }

    ///
    /// \brief Set the maximum allowed acceleration
    ///
    void set_gain(real_t k){k_ = k;}

    ///
    /// \brief Set the waypoint radius
    ///
    void set_wpoint_radius(real_t r){waypoint_r_ = r;}

    ///
    /// \brief Update. Notify the observer that the
    /// resource is observing has been changed
    ///
    virtual void update(const path_t& resource) override final;

    ///
    /// \brief Read the resource
    ///
    virtual const path_t& read()const override final;

    ///
    /// \brief Returns this as ObserverBase
    ///
    obsertver_base_t& get_observer(){return *this;}

    ///
    /// \brief Returns this as ObserverBase
    ///
    const obsertver_base_t& get_observer()const{return *this;}

private:

    ///
    /// \brief The gain parameter
    ///
    real_t k_;

    ///
    /// \brief The lookahead distance
    ///
    real_t lookahead_distance_;

    ///
    /// \brief number of sampling poinst to use
    /// when computing the closest point from
    /// position to the path
    ///
    uint_t n_sampling_points_;

    ///
    /// \brief Tolerance used for calculations
    ///
    real_t tol_;

    ///
    /// \brief Radius within which it is
    /// assumed that the waypoint has been reached
    ///
    real_t waypoint_r_;

    typedef typename path_t::element_t element_t;

    ///
    /// \brief The current segment the algorithm is using
    ///
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
CarrotChasingPathTrackController<PointData,
                                SegmentData>::CarrotChasingPathTrackController(const CarrotChasingPathTrackControllerInput& input )
    :
   kernel::ObserverBase<grids::WaypointPath<2, PointData, SegmentData>*>(),
   k_(input.k),
   lookahead_distance_(input.lookahead_distance),
   n_sampling_points_(input.n_sampling_points),
   tol_(input.tol),
   waypoint_r_(input.waypoint_r),
   current_element_(nullptr)
{}
template<typename PointData, typename SegmentData>
std::tuple<real_t, kernel::GeomPoint<2>, kernel::GeomPoint<2>>
CarrotChasingPathTrackController<PointData,
SegmentData>::execute(const kernel::dynamics::SysState<3>& state)const{

    CarrotChasingPathTrackController<PointData,SegmentData>::result_t result;

    const kernel::GeomPoint<2> p({state.get("X"), state.get("Y")});
    const real_t theta = state.get("Theta");

    // find out the segment at which
    // we are closest to
    const path_t& path=this->read();

    auto [closest_path_point, segment] = kernel::find_closest_point_to(path, p, n_sampling_points_, tol_);

    if(segment == nullptr){
         /// we cannot proceed
         throw std::logic_error("No segment found for lookahead point");
     }

    current_element_ = segment;
    // find the look ahead point
    auto [found, lookahead_point] = kernel::find_point_on_line_distant_from_p(*current_element_,
                                                                              closest_path_point,
                                                                              lookahead_distance_,
                                                                               n_sampling_points_,
                                                                               tol_, true);
    // if we were not able to find a lookahead point
    // on the current_element using lookahead_distance_
    if(!found){

           /// iteratively look on the next segments until
           /// we exhaust the path
           while(!found){

               /// if this is the last segment on the path then
               /// the best option is to simply use the end vertex
               if(current_element_->get_id() == path.n_elements()-1){
                   /// the lookahed point is the next waypoint
                   lookahead_point = current_element_->get_vertex(1);
                   break;
               }

               current_element_ = path.element(current_element_->get_id() + 1);
               auto cpp = current_element_->start();

               auto [f, l_point] = kernel::find_point_on_line_distant_from_p(*current_element_,
                                                                                         cpp,
                                                                                         lookahead_distance_,
                                                                                          n_sampling_points_,
                                                                                          tol_, true);
                found = f;
                lookahead_point = l_point;

           }

    }

    real_t theta_d = std::atan2(lookahead_point[1] - p[1], lookahead_point[0] - p[0]);
    return {k_*(theta_d - theta), lookahead_point, closest_path_point};

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
