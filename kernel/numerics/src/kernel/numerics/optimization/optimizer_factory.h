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

    template<typename DatasetTp, typename FunctionTp, typename SolverOps>
    auto build(OptimizerType type, const SolverOps& options) -> std::shared_ptr<OptimizerBase<DatasetTp, FunctionTp>>;

};

template<typename DatasetTp, typename FunctionTp, typename SolverOps>
auto
OptimizerFactory::build(OptimizerType type, const SolverOps& options) -> std::shared_ptr<OptimizerBase<DatasetTp, FunctionTp>>{

    auto ptr = std::shared_ptr<OptimizerBase<DatasetTp, FunctionTp>>(nullptr);
    switch(type){

        case OptimizerType::GD:
        {
            ptr = std::make_shared<Gd<DatasetTp, FunctionTp>>(options);
            break;
        }
        case OptimizerType::SGD:
        {
            ptr = std::make_shared<SGD<DatasetTp, FunctionTp>>(options);
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
