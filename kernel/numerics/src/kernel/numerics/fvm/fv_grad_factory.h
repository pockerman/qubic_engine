#ifndef FV_GRAD_FACTORY_H
#define FV_GRAD_FACTORY_H

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

#endif // FV_GRAD_FACTORY_H
