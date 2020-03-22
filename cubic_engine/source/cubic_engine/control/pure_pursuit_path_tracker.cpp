#include "cubic_engine/control/pure_pursuit_path_tracker.h"
#include "kernel/discretization/utils/line_mesh_utils.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/geometry/shapes/circle.h"
#include "kernel/utilities/common_uitls.h"

#include <cmath>
#include <stdexcept>

namespace cengine{
namespace control{

PurePursuit2DPathTracker::PurePursuit2DPathTracker()
    :
      kernel::ObserverBase<kernel::numerics::LineMesh<2>*>(),
      boost::noncopyable(),
      lookahead_distance_(0.0),
      goal_radius_(0.0),
      goal_(),
      n_sampling_points_(kernel::KernelConsts::invalid_size_type())
{}

void
PurePursuit2DPathTracker::update(const path_t& resource){
    this->kernel::ObserverBase<kernel::numerics::LineMesh<2>*>::update(resource);
}

void
PurePursuit2DPathTracker::read(path_t& r)const{
  this->kernel::ObserverBase<kernel::numerics::LineMesh<2>*>::read(r);
}


std::tuple<kernel::GeomPoint<2>, real_t>
PurePursuit2DPathTracker::execute(const kernel::dynamics::SysState<3>& state){

    /// 1. Find the path point closest to the position

    /// get the position coordinates
    real_t rx = state.get("X");
    real_t ry = state.get("Y");

    /// form the position
    kernel::GeomPoint<2> position({rx, ry});

    //path_t path;
    //this->read(path);

    /// find the closest point from the position to the
    /// path
    //const kernel::GeomPoint<2> point = kernel::numerics::find_closest_point_to(path, position,
    //                                                                           n_sampling_points_,
    //                                                                           kernel::KernelConsts::tolerance());

    /// 2. Find the lookahead point. We can find the lookahead point
    /// by finding the intersection point of the circle centered at
    /// the robot's location and radius equal to the lookahead distance
    /// and the path segment
    /*auto intersections = kernel::numerics::find_intersections(path,
                                                              kernel::Circle(lookahead_distance_, position));

    if(intersections.empty()){
        /// we cannot proceed
        throw std::logic_error("No intersection points found");
    }

    ///3. calculate the curvature
    /// this will be
    auto theta = state.get("Theta");
    auto a = std::tan(theta);
    auto b = 1.0;
    auto c = a*rx - ry;
    auto lookahead_point = intersections[0];

    auto x = std::fabs(a*lookahead_point[0] +
            b*lookahead_point[0] +c)/std::sqrt(a*a + b*b);

    auto k = 2*x/kernel::utils::sqr(lookahead_distance_*lookahead_distance_);

    /// we need however a signed curvature

    /// construct a point on the robot line
    real_t bx = rx + std::cos(theta);
    real_t by = ry + std::sin(theta);

    auto sign = kernel::utils::sign(std::sin(theta)*(rx - lookahead_point[0]) -
            std::cos(theta)*(ry - lookahead_point[1]));*/


    return std::make_tuple(kernel::GeomPoint<2>(), 1);
}

}
}
