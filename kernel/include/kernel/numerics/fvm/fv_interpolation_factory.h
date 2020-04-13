#ifndef FV_INTERPOLATION_FACTORY_H
#define FV_INTERPOLATION_FACTORY_H

#include "kernel/numerics/fvm/fv_interpolation_types.h"
#include <memory>

namespace kernel{
namespace numerics{

template<int dim> class FVInterpolateBase;

template<int dim>
struct FVInterpolationFactory
{
    static std::shared_ptr<FVInterpolateBase<dim>> build(FVInterpolationType type);
};

}
}

#endif // FV_INTERPOLATION_FACTORY_H
