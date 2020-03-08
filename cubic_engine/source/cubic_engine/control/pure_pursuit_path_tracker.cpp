#include "cubic_engine/control/pure_pursuit_path_tracker.h"

namespace cengine{
namespace control{

PurePursuit2DPathTracker::PurePursuit2DPathTracker()
    :
      kernel::ObserverBase<kernel::numerics::LineMesh<2>*>(),
      boost::noncopyable()
{}

void
PurePursuit2DPathTracker::update(const path_t& resource){
    this->kernel::ObserverBase<kernel::numerics::LineMesh<2>*>::update(resource);
}

void
PurePursuit2DPathTracker::read(path_t& r)const{
  this->kernel::ObserverBase<kernel::numerics::LineMesh<2>*>::read(r);
}


void
PurePursuit2DPathTracker::execute(const kernel::dynamics::SysState<2>& state){

    // 1. Find the path point closest to the position
    real_t x = state.get("X");
    real_t y = state.get("Y");

    kernel::GeomPoint<2> position({x,y});

    path_t path;
    this->read(path);

    //
    const kernel::GeomPoint<2> = path.find_closest_point_to(position);

    //2. Find the lookahead point

    //3. calculate the curvature and return the steering
    //   request based on that curvature
}


}

}
