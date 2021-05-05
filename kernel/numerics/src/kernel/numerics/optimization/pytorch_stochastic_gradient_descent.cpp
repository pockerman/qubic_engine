#include "kernel/numerics/optimization/pytorch_stochastic_gradient_descent.h"

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

namespace kernel{
namespace numerics {
namespace opt{
namespace pytorch{


PYT_StochasticGD::PYT_StochasticGD(const GDConfig& control)
    :
    control_(control)
{}


}
}
}
}
#endif
