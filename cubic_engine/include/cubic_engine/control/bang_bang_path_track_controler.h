#ifndef BANG_BANG_PATH_TRACK_CONTROLER_H
#define BANG_BANG_PATH_TRACK_CONTROLER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/grids/waypoint_path.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/patterns/observer_base.h"

#include "boost/noncopyable.hpp"
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


    /// \brief Execute controller
    void execute(const kernel::dynamics::SysState<3>& state)const;


private:

    /// \brief The fitting coefficient
    real_t cx_;

    /// \brief Maximum acceleration
    real_t amax_;

    /// \brief The lookahead distance
    real_t lookahead_dist_;

    /// \brief number of sampling poinst to use
    /// when computing the closest point from
    /// position to the path
    uint_t n_sampling_points_;

    /// \brief The lookahed point calculated
    /// by the controller
    kernel::GeomPoint<2> lookahead_point_;
};

}

}

#endif // BANG_BANG_PATH_TRACK_CONTROLER_H
