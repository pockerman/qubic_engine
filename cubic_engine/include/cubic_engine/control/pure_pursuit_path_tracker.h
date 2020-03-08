#ifndef PURE_PURSUIT_PATH_TRACKER_H
#define PURE_PURSUIT_PATH_TRACKER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/discretization/line_mesh.h"
#include "kernel/patterns/observer_base.h"

#include "boost/noncopyable.hpp"

namespace cengine {
namespace control {

class PurePursuit2DPathTracker: public kernel::ObserverBase<kernel::numerics::LineMesh<2>*>,
                                private boost::noncopyable
{

public:

    /// \brief Expose the type of the path
    typedef kernel::ObserverBase<kernel::numerics::LineMesh<2>*>::resource_t  path_t;


    /// \brief Constructor
    PurePursuit2DPathTracker();

    /// \brief Execute the control
    void execute(const kernel::dynamics::SysState<2>& state);

    /// \brief Set the lookahead distance
    void set_lookahead_dist(real_t dist){lookahead_distance_ = dist;}

    /// \brief Set the goal radius
    void set_goal_radius(real_t r){goal_radius_ = r;}

    /// \brief Update. Notify the observer that the
    /// resource is observing has been changed
    virtual void update(const path_t& resource) override final;


private:

    /// \brief Read the resource
    virtual void read(path_t& r)const override final;

    /// \brief The lookahead distance paramter
    real_t lookahead_distance_;

    /// \brief The radius of the circle within which
    /// the goal is assumed to lie
    real_t goal_radius_;

    /// \brief The goal location
    kernel::GeomPoint<2> goal_;


};

}

}

#endif // PURE_PURSUIT_PATH_TRACKER_H
