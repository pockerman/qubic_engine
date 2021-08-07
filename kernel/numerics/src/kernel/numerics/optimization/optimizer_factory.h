#ifndef OPTIMIZER_FACTORY_H
#define OPTIMIZER_FACTORY_H

#include "kernel/base/config.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

#include "kernel/numerics/optimization/optimizer_type.h"
#include "kernel/numerics/optimization/optimizer_base.h"
#include "kernel/numerics/optimization/serial_gradient_descent.h"
#include "kernel/numerics/optimization/stochastic_gradient_descent.h"

#include <map>
#include <any>
#include <string>
#include <memory>

namespace kernel {
namespace numerics {
namespace opt {


class OptimizerFactory
{
public:

    template<typename MatTp, typename VecTp, typename SolverOps>
    auto build(OptimizerType type, const SolverOps& options) -> std::shared_ptr<OptimizerBase<MatTp, VecTp>>;

};

template<typename MatTp, typename VecTp, typename SolverOps>
auto
OptimizerFactory::build(OptimizerType type, const SolverOps& options) -> std::shared_ptr<OptimizerBase<MatTp, VecTp>>{

    auto ptr = std::shared_ptr<OptimizerBase<MatTp, VecTp>>(nullptr);
    switch(type){

        case OptimizerType::GD:
        {
            ptr = std::make_shared<Gd<MatTp, VecTp>>(options);
            break;
        }
        case OptimizerType::SGD:
        {
            ptr = std::make_shared<SGD<MatTp, VecTp>>(options);
            break;
        }
#ifdef KERNEL_DEBUG
        default:
        {
            assert(false && "Unrecognized OptimizerType");
            ptr = nullptr;
        }
#endif
    }

    return ptr;
}

}

}

}

#endif // OPTIMIZER_FACTORY_H
