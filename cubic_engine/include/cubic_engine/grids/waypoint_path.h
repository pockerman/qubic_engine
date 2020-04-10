#ifndef WAYPOINT_PATH_H
#define WAYPOINT_PATH_H
#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/geometry/generic_line.h"

#include <vector>
#include <stdexcept>

namespace cengine{
namespace grids {

/// \brief Helper class to represent a waypoint
template<int dim, typename Data>
struct WayPoint
{
    typedef kernel::GeomPoint<dim> position_t;
    typedef Data data_t;
    position_t position;
    uint_t id;
    Data data;

    /// constructor
    WayPoint(const position_t& p, uint_t id_, const data_t& data_=data_t())
        :
          position(p),
          id(id_),
          data(data)
    {}
};

struct LineSegmentData
{
    /// \brief The maximum velocity
    /// allowed on the edge
    real_t Vmax;
};

template<int dim, typename NodeData, typename SegmentData>
class LineSegment: private kernel::kernel_detail::generic_line_base<WayPoint<dim, NodeData>>
{
public:

    typedef NodeData node_data_t;
    typedef SegmentData segment_data_t;
    typedef WayPoint<dim, NodeData> w_point_t;
    typedef kernel::kernel_detail::generic_line_base<w_point_t> base;
    typedef typename base::node_type node_type;

    using base::start;
    using base::end;
    using base::get_id;
    using base::set_id;
    using base::has_valid_id;

    /// \brief Constructor
    LineSegment(uint_t id, const w_point_t& v1,
                const w_point_t& v2, const segment_data_t& data);

private:

    /// \brief list of internal points of
    /// the segment.
    std::vector<w_point_t> internal_points_;

    /// \brief The data asociated with the segmen
    segment_data_t data_;

};

template<int dim, typename NodeData, typename SegmentData>
LineSegment<dim, NodeData, SegmentData>::LineSegment(uint_t id,
                                                     const typename LineSegment<dim, NodeData, SegmentData>::w_point_t& v1,
                                                     const typename LineSegment<dim, NodeData, SegmentData>::w_point_t& v2,
                                                     const typename LineSegment<dim, NodeData, SegmentData>::segment_data_t& data)
    :
     kernel::kernel_detail::generic_line_base<WayPoint<dim, NodeData>>(v1, v2, id),
     internal_points_(),
     data_(data)
{}

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

    /// \brief How many waypoints the pah has
    uint_t n_wpoints()const{return waypoints_.size();}

    /// \brief How many segments the path has
    uint_t n_segments()const{return segments_.size();}

    /// \brief Reserve space for waypoints
    void reserve_n_waypoints(uint_t n);

    /// \brief Reserve space for
    void reserve_n_segments(uint_t n);

    /// \brief Add a new waypoint in the path and get back
    /// a writable reference of the newly added point
    w_point_t& add_way_point(const kernel::GeomPoint<dim>& position,
                             const w_point_data_t& data);

    /// \brief Add a new segment in the path and get back
    /// a writable reference of the newly added segment.
    /// Throws std::logic_error if vid0 and vid1 are not
    /// in the path
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

template<int dim, typename PointData, typename EdgeData>
WaypointPath<dim, PointData, EdgeData>::WaypointPath()
    :
      waypoints_(),
      segments_()
{}

template<int dim, typename PointData, typename EdgeData>
typename WaypointPath<dim, PointData, EdgeData>::w_point_t&
WaypointPath<dim, PointData, EdgeData>::add_way_point(const kernel::GeomPoint<dim>& position,
                                                      const typename WaypointPath<dim, PointData, EdgeData>::w_point_data_t& data){

    uint_t id = waypoints_.size();
    WaypointPath<dim, PointData, EdgeData>::w_point_t p(position, id, data);
    waypoints_.push_back(p);
    return waypoints_[id];
}

template<int dim, typename PointData, typename EdgeData>
typename WaypointPath<dim, PointData, EdgeData>::segment_t&
WaypointPath<dim, PointData, EdgeData>::add_segment(uint_t vid0, uint_t vid1,
                            const typename WaypointPath<dim, PointData, EdgeData>::segment_data_t& data){

    if(vid0 >= waypoints_.size() ||
            vid1 >= waypoints_.size()){

        throw std::logic_error("Waypoint id given not in range [0, " +
                               std::to_string(waypoints_.size()) + ")");

    }

    if(vid0 == vid1){
       throw std::logic_error("Cannot create segment having identical vertices");
    }

    auto id = segments_.size();
    auto v0 = waypoints_[vid0];
    auto v1 = waypoints_[vid1];
    WaypointPath<dim, PointData, EdgeData>::segment_t seg(id, v0, v1, data);
    segments_.push_back(seg);
    return segments_[id];
}

template<int dim, typename PointData, typename EdgeData>
void
WaypointPath<dim, PointData, EdgeData>::reserve_n_waypoints(uint_t n){
    waypoints_.reserve(n);
}

template<int dim, typename PointData, typename EdgeData>
void
WaypointPath<dim, PointData, EdgeData>::reserve_n_segments(uint_t n){
    segments_.reserve(n);
}

}
}

#endif // WAYPOINT_PATH_H
