#include "kernel/base/types.h"
#include "kernel/discretization/line_mesh.h"
#include "kernel/discretization/mesh_generation.h"
#include "kernel/geometry/geom_point.h"




#include <iostream>
#include <mutex>

namespace example
{

using kernel::real_t;
using kernel::uint_t;
using DynMat = kernel::DynMat<real_t>;
using DynVec = kernel::DynVec<real_t>;
using kernel::numerics::LineMesh;

}


int main(){

    using namespace example;

    LineMesh mesh;

    return 0;
}




