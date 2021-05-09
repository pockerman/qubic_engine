#ifndef DUMMY_ENVIRONMENT_DYNAMICS_H
#define DUMMY_ENVIRONMENT_DYNAMICS_H

#include "cubic_engine/base/cubic_engine_types.h"

namespace cengine{
namespace rl{

///
/// \brief The DymmyEnvironmentDynamics class. Helper
/// modeling environment dynamics that we want to have no effect.
/// This is actually just a placeholder
///
class DymmyEnvironmentDynamics
{
public:

    ///
    /// \brief ConstantEnvironmentDynamics Constructor
    ///
    DymmyEnvironmentDynamics()=default;

    ///
    /// \brief operator () Return the constant dynamics value
    ///
    template<typename StateTp, typename ActionTp>
    real_t operator()(const StateTp& /*state*/, const ActionTp& /*action*/)const{return 0.0;}

};

}
}

#endif // DUMMY_ENVIRONMENT_DYNAMICS_H
