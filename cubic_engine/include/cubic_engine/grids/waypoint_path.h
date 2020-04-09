#ifndef WAYPOINT_PATH_H
#define WAYPOINT_PATH_H
#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/geometry/generic_line.h"

#include <vector>
namespace cengine{
namespace grids {

/// \brief Helper class to represent a waypoint
template<int dim, typename Data>
struct WayPoint
{
    kernel::GeomPoint<dim> position;
    uint_t id;
    Data data;

};

template<int dim, typename NodeData, typename SegmentData>
class LineSegment: private kernel::kernel_detail::generic_line_base<WayPoint<dim, NodeData>>
{
public:

    typedef WayPoint<dim, NodeData> w_point_t;
    typedef kernel::kernel_detail::generic_line_base<w_point_t> base;
    typedef typename base::node_type node_type;

    using base::start;
    using base::end;
    using base::get_id;
    using base::set_id;
    using base::has_valid_id;

private:


    /// \brief list of internal points of
    /// the segment.
    std::vector<w_point_t> internal_points_;

};

/// \brief class WaypointPath models a path formed
/// by line segments and way points. The Data
/// template parameter represents the data held
/// at the waypoints of the path
template<int dim, typename PointData, typename EdgeData>
class WaypointPath
{
public:

    typedef PointData w_point_data_t;
    typedef WayPoint<dim, w_point_data_t> w_point_t;
    typedef EdgeData segment_data_t;
    typedef LineSegment<dim, w_point_data_t, segment_data_t> segment_t;

    /// \brief point iteration
    typedef typename std::vector<w_point_t>::iterator node_iterator_impl;
    typedef typename std::vector<w_point_t>::const_iterator cnode_iterator_impl;

    /// \brief Line segment  iteration
    typedef typename std::vector<segment_t>::iterator element_iterator_impl;
    typedef typename std::vector<segment_t>::const_iterator celement_iterator_impl;

    /// \brief Constructor
    WaypointPath();

    /// \brief Reserve space for waypoints
    void reserve_n_waypoints(uint_t n);

    /// \brief Reserve space for
    void reserve_n_segments(uint_t n);


    /// \brief Add a new waypoint in the path and get back
    /// a writable reference of the newly added point
    w_point_t& add_way_point(const kernel::GeomPoint<dim>& position,
                             const w_point_data_t& data);

    /// \brief Add a new segment in the path and get back
    /// a writable reference of the newly added segment
    segment_t& add_segment(uint_t vid0, uint_t vid1,
                            const segment_data_t& data);

    /// \brief Raw node iteration
    node_iterator_impl nodes_begin(){return waypoints_.begin();}
    node_iterator_impl nodes_end(){return waypoints_.end();}

    /// \brief Raw node iteration
    cnode_iterator_impl nodes_begin()const{return waypoints_.begin();}
    cnode_iterator_impl nodes_end()const{return waypoints_.nodes_end();}

    /// \brief Raw elements iteration
    element_iterator_impl elements_begin(){return segments_.begin();}
    element_iterator_impl elements_end(){return segments_.end();}

    /// \brief Raw elements iteration
    celement_iterator_impl elements_begin()const{return segments_.begin();}
    celement_iterator_impl elements_end()const{return segments_.end();}

private:

    /// \brief The Waypoints of the path
    std::vector<w_point_t> waypoints_;

    /// \brief The segments of the path
    std::vector<segment_t> segments_;

};

}
}

#endif // WAYPOINT_PATH_H
