#ifndef FV_GRAD_FACTORY_H
#define FV_GRAD_FACTORY_H

#include "kernel/base/config.h"

#ifdef USE_FVM

#include "kernel/numerics/fvm/fv_grad_types.h"
#include <memory>

namespace kernel {
namespace numerics {

template<int dim> class FVGradBase;

template<int dim>
struct FVGradFactory
{
    static std::shared_ptr<FVGradBase<dim>> build(FVGradType type);
};

}

}

#endif
#endif // FV_GRAD_FACTORY_H
