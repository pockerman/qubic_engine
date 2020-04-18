#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/geometry/generic_line.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/geometry/shapes/circle.h"

#include <stdexcept>
#include <cmath>
#include <utility>
#include <tuple>

namespace kernel{

const GeomPoint<3>
cross_product(const GeomPoint<3>& o1, const GeomPoint<3>& o2);

const GeomPoint<3>
cross_product(const GeomPoint<2>& o1, const GeomPoint<2>& o2);


/// \brief Returns the GeomPoint in the
/// given container that has the minimum distance
/// from the given point
template<int dim, typename ContainerTp>
const GeomPoint<dim>
get_point_with_min_distance(const GeomPoint<dim>& p,
                            const ContainerTp& c,
                            real_t tol= KernelConsts::tolerance()){

    if(c.empty()){
        throw std::logic_error("Empty points list");
    }

    auto current_dist = p.distance(c[0]);
    auto current_point = c[0];

    for(uint_t i=1; i<c.size(); ++i){

        auto new_dist = p.distance(c[i]);

        if( current_dist - new_dist > 0.0 ){
            current_dist = new_dist;
            current_point = c[i];
        }
    }

    return current_point;
}

std::pair<bool, real_t>
has_intersection(real_t discriminant, real_t b, real_t a);

template<typename LineTp>
const kernel::GeomPoint<LineTp::dimension>
find_point_on_line_closest_to(const LineTp& element, const GeomPoint<LineTp::dimension>& p,
                                 uint_t nsamples, real_t tol){

    static_assert (LineTp::dimension == 2, "Line dimension is not 2");
    typedef kernel::GeomPoint<LineTp::dimension> point_t;

    std::vector<point_t> points_on_segment(nsamples + 2);

    auto v0 = element.get_vertex(0);
    auto v1 = element.get_vertex(1);

    real_t distance = v0.distance(v1);

    ///...plus 2 because we also account for the end poinst
    real_t h = distance/(real_t) (nsamples + 2);

    /// the first point is the staring vertex
    points_on_segment[0] = v0;

    /// the first point is the staring vertex
    points_on_segment[0] = v0;

    if(std::fabs(v1[0] - v0[0]) < tol){
        /// we have line that is perpendicular to the
        /// x-axis and we need to act differently to avoid infinity

        for(uint_t sp = 1; sp <=nsamples; ++sp){

            real_t xi = v0[0];
            points_on_segment[sp] = point_t({xi, v0[0] + sp*h});
        }
    }
    else if(std::fabs(v1[1] - v0[1]) < tol){
        /// we have line that is parallel to the
        /// x-axis and we need to act differently to avoid infinity

        for(uint_t sp = 1; sp <=nsamples; ++sp){

            real_t xi = v0[1];
            points_on_segment[sp] = point_t({v0[0] + sp*h, xi});
        }
    }
    else{

        /// calculate the coefficients of the line
        /// connecting the two vertices
        real_t alpha = (v0[1] - v1[1])/(v0[0] - v1[0]);
        real_t beta = ((v1[1]*v0[0] - v0[1]*v1[0]) )/(v0[0] - v1[0]);

        for(uint_t sp = 1; sp <=nsamples; ++sp){

            real_t xi = v0[0] + sp*h;
            points_on_segment[sp] = point_t({xi, alpha*xi + beta});
        }
    }
    /// add the ending vertex of the line segment
    points_on_segment[nsamples + 1] = v1;

    auto min_point = get_point_with_min_distance(p, points_on_segment);
    return min_point;
}

template<typename LineTp>
std::tuple<bool, GeomPoint<LineTp::dimension>>
find_point_on_line_at_delta_distance(const LineTp& line,
                                     real_t delta, uint_t nsamples,
                                     real_t tol){

    static_assert (LineTp::dimension == 2, "Line dimension is not 2");
    typedef kernel::GeomPoint<LineTp::dimension> point_t;
    std::vector<point_t> points_on_segment(nsamples + 2);

    auto& v0 = line.get_vertex(0);
    auto& v1 = line.get_vertex(1);

    real_t distance = v0.distance(v1);

    ///...plus 2 because we also account for the end poinst
    real_t h = distance/(real_t) (nsamples + 2);

    /// the first point is the staring vertex
    points_on_segment[0] = v0;

    /// the first point is the staring vertex
    points_on_segment[0] = v0;

    if(std::fabs(v1[0] - v0[0]) < tol){
        /// we have line that is perpendicular to the
        /// x-axis and we need to act differently to avoid infinity

        for(uint_t sp = 1; sp <=nsamples; ++sp){
            real_t xi = v0[0];
            points_on_segment[sp] = point_t({xi, v0[0] + sp*h});
        }
    }
    else if(std::fabs(v1[1] - v0[1]) < tol){
        /// we have line that is parallel to the
        /// x-axis and we need to act differently to avoid infinity

        for(uint_t sp = 1; sp <=nsamples; ++sp){
            real_t xi = v0[1];
            points_on_segment[sp] = point_t({v0[0] + sp*h, xi});
        }
    }
    else{

        /// calculate the coefficients of the line
        /// connecting the two vertices
        real_t alpha = (v0[1] - v1[1])/(v0[0] - v1[0]);
        real_t beta = ((v1[1]*v0[0] - v0[1]*v1[0]) )/(v0[0] - v1[0]);

        for(uint_t sp = 1; sp <=nsamples; ++sp){

            real_t xi = v0[0] + sp*h;
            points_on_segment[sp] = point_t({xi, alpha*xi + beta});
        }
    }
    /// add the ending vertex of the line segment
    points_on_segment[nsamples + 1] = v1;

    for(auto& p:points_on_segment){
        if(p.distance(v0) > delta){
            return {true, p};
        }
    }

    return {false, point_t()};
}

/// \brief Given a line, a point p on the line
/// and a distance delta returns a point that is
/// on the line and delta apart from p
template<typename LineTp>
std::tuple<bool,GeomPoint<LineTp::dimension>>
find_point_on_line_distant_from_p(const LineTp& line,
                                  const GeomPoint<LineTp::dimension>& p,
                                  real_t delta, uint_t nsamples, real_t tol){

    auto& v0 = line.get_vertex(0);
    auto& v1 = line.get_vertex(1);

    /// check if we can construct the point
    if(v0.distance(p) < 0.0){
        if(v0.distance(p) < std::fabs(delta)){
            return {false, GeomPoint<LineTp::dimension>()};
        }
    }
    else if(v1.distance(p) < delta){
        return {false, GeomPoint<LineTp::dimension>()};
    }

    if(delta < 0.0){
       GenericLine<GeomPoint<LineTp::dimension>, void> newline(p, v0);
       return find_point_on_line_at_delta_distance(newline, std::fabs(delta), nsamples, tol);
    }

    GenericLine<GeomPoint<LineTp::dimension>, void> newline(p, v1);
    return find_point_on_line_at_delta_distance(newline, delta, nsamples, tol);


}

template<typename LineTp>
real_t
distance_from(const LineTp& element,
              const GeomPoint<LineTp::dimension>& p,
              uint_t nsamples, real_t tol){


    auto point = find_point_on_line_closest_to(element, p, nsamples, tol);
    return  point.distance(p);
}

/// \brief Find the element in the mesh
/// with the smallest distance from the point
template<typename MeshTp>
const typename MeshTp::element_t*
find_closest_element(const MeshTp& mesh, const GeomPoint<MeshTp::dimension>& p,
                     uint_t nsamples, real_t tol){

    numerics::ConstElementMeshIterator<numerics::Active, MeshTp> filter(mesh);
    typedef typename MeshTp::element_t element_t;
    element_t* ptr = nullptr;
    real_t dist = std::numeric_limits<real_t>::max();

    auto begin = filter.begin();
    auto end   = filter.end();

    for(; begin != end; ++begin){

        auto element = *begin;
        auto dist_cur = distance_from(*element, p, nsamples, tol);

        if(dist_cur < dist){
            dist = dist_cur;
            ptr = element;
        }
    }

    return ptr;
}

template<typename MeshTp>
std::tuple<const typename MeshTp::point_t, const typename MeshTp::element_t*>
find_closest_point_to(const MeshTp& mesh, const GeomPoint<2>& p,
                      uint_t nsamples, real_t tol){

    static_assert (MeshTp::dimension == 2, "Mesh dimension is not 2");

    typedef typename MeshTp::point_t point_t;

    std::vector<point_t> min_points;
    min_points.reserve(mesh.n_elements());

    numerics::ConstElementMeshIterator<numerics::Active, MeshTp> filter(mesh);

    auto begin = filter.begin();
    auto end   = filter.end();

    for(; begin != end; ++begin){

        auto element = *begin;
        auto min_point = find_point_on_line_closest_to(*element, p, nsamples, tol);
        min_points.push_back(min_point);
    }

    auto result = get_point_with_min_distance(p, min_points);

    /// find out to which segment this point belongs to
    auto element = find_closest_element(mesh, result, nsamples, tol);

    return {result, element};
}

/// \brief Returns the intersection points of the
/// Circle with the elements of the LineMesh.
/// The algorithn implemented is the one described here
/// https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm/1084899#1084899%E2%80%8B
template<typename MeshTp>
const std::vector<typename MeshTp::point_t> find_intersections(const MeshTp& mesh,
                                                                 const Circle& circle){

    static_assert (MeshTp::dimension == 2, "Mesh dimension is not 2");

    std::vector<typename MeshTp::point_t> intersections;

    const auto r = circle.radius();
    const auto center = circle.center();

    numerics::ConstElementMeshIterator<numerics::Active, MeshTp> filter(mesh);
    auto begin = filter.begin();
    auto end   = filter.end();

    for(; begin != end; ++begin){

        auto element = *begin;
        auto v0 = element->get_vertex(0);
        auto v1 = element->get_vertex(1);

        auto d = v1 - v0;
        auto f = v0 - center;

        auto a = d.dot(d);
        auto b = 2.0*f.dot(d);
        auto c = f.dot(f) - r*r;

        auto discriminant = b*b-4*a*c;

        auto intersects = has_intersection(discriminant, b, a);
        if(intersects.first){

            /// thats an intersection point
            auto intersection = v0  + d*intersects.second;
            intersections.push_back(intersection);
        }
    }

    return intersections;
}




}

#endif // GEOMETRY_UTILS_H
