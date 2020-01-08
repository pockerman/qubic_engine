#ifndef PYTORCH_STOCHASTIC_GRADIENT_DESCENT_H
#define PYTORCH_STOCHASTIC_GRADIENT_DESCENT_H

#include "cubic_engine/base/config.h"

#ifdef USE_PYTORCH


#include "cubic_engine/optimization/utils/gd_control.h"
#include "cubic_engine/optimization/utils/gd_info.h"

#include <torch/optim/sgd.h>
namespace cengine
{

namespace pytorch
{

/// \brief PY_StochasticDG class. Wrapper to PyTorch Stochastic Gradient Descent algorithm
class PY_StochasticDG
{

public:


    PY_StochasticDG(const GDControl& control);


    /**
     * @brief Solves the optimization problem. Returns information
     * about the performance of the solver.
     */
    template<typename MatType, typename VecType,
             typename ErrorFuncType, typename HypothesisFuncType>
    GDInfo solve(const MatType& mat,const VecType& v,
                 ErrorFuncType& error_fun, HypothesisFuncType& h);


private:

    /// \brief Control of the algorithm
    GDControl control_;

};

template<typename MatType, typename VecType,
         typename ErrorFuncType, typename HypothesisFuncType>
GDInfo
PY_StochasticDG::solve(const MatType& mat,const VecType& v,
                       ErrorFuncType& error_fun, HypothesisFuncType& h){


    torch::optim::SGD sgd;

    while(control_.continue_iterations()){

        // Clear gradient buffers because we don't want any
        // gradient from previous epoch to carry forward, dont want to cummulate gradients
        sgd.zero_grad();

        // get output from the model, given the inputs
        auto outputs = h(inputs);

        // get loss for the predicted output
        auto loss = error_fun(outputs, labels);

        //  get gradients w.r.t to parameters
        loss.backward();

        // do a step in the algorithm
        sgd.step();
    }
}


}

}
#endif
#endif // PYTORCH_STOCHASTIC_GRADIENT_DESCENT_H
