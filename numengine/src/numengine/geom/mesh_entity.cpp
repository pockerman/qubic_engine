#include "numengine/geom/mesh_entity.h"

namespace numengine
{

#ifdef NUMENGINE_DIM_1
 template class MeshEntity<1,0>;
 template class MeshEntity<1,1>;
#endif

#ifdef NUMENGINE_DIM_2
 template class MeshEntity<2,0>;
 template class MeshEntity<2,1>;
 template class MeshEntity<2,2>;
#endif

#ifdef NUMENGINE_DIM_3
 template class MeshEntity<3,0>;
 template class MeshEntity<3,1>;
 template class MeshEntity<3,2>;
 template class MeshEntity<3,3>;
#endif

}//libsimpp
