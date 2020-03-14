#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/geometry/geom_point.h"
#include <stdexcept>

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
        if( current_dist - new_dist < tol){
            current_dist = new_dist;
            current_point = c[i];
        }
    }

    return current_point;
}

}

#endif // GEOMETRY_UTILS_H
