#ifndef CONSTANT_ENVIRONMENT_DYNAMICS_H
#define CONSTANT_ENVIRONMENT_DYNAMICS_H

#include "cubic_engine/base/cubic_engine_types.h"

namespace cengine {
namespace rl{

///
/// \brief The ConstantEnvironmentDynamics class. Helper
/// modeling environment dynamics where all states have
/// the same probability to occur
///
class ConstantEnvironmentDynamics
{
public:

    ///
    /// \brief ConstantEnvironmentDynamics Constructor
    ///
    ConstantEnvironmentDynamics()=default;

    ///
    /// \brief ConstantEnvironmentDynamics Constructor
    ///
    ConstantEnvironmentDynamics(real_t val);

    ///
    /// \brief operator () Return the constant dynamics value
    ///
    template<typename StateTp, typename ActionTp>
    real_t operator()(const StateTp& state, const ActionTp& action)const{return val_;}

    ///
    /// \brief set_value. Set the constant dynamics value
    ///
    void set_value(const real_t val){val_ = val;}

private:

    ///
    /// \brief val_ Constant dynamics value
    ///
    real_t val_;

};

inline
ConstantEnvironmentDynamics::ConstantEnvironmentDynamics(real_t val)
    :
      val_(val)
{}

}

}
#endif // CONSTANT_ENVIRONMENT_DYNAMICS_H
