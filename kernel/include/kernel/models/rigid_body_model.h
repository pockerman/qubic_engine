#ifndef RIGID_BODY_MODEL_H
#define RIGID_BODY_MODEL_H
#include "kernel/base/types.h"

namespace kernel {
namespace models {

///
/// \brief RigidBodyModel class. Base class for deriving
/// rigid body models
///
template<typename ChassisTp, typename DynamicsTp>
class RigidBodyModel: public ChassisTp
{
public:

    typedef ChassisTp chassis_t;
    typedef DynamicsTp dynamics_t;

    ///
    /// \brief RigidBodyModel. Constructor
    ///
    RigidBodyModel();


};

}

}

#endif // RIGID_BODY_MODEL_H
