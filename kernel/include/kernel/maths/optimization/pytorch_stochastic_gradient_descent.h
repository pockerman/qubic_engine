#ifndef PYTORCH_STOCHASTIC_GRADIENT_DESCENT_H
#define PYTORCH_STOCHASTIC_GRADIENT_DESCENT_H

#include "cubic_engine/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/optimization/utils/gd_control.h"
#include "cubic_engine/optimization/utils/gd_info.h"

#include <torch/optim/sgd.h>

#include <boost/noncopyable.hpp>
#include <chrono>
#include <iostream>
#include <vector>

namespace cengine
{

namespace pytorch
{

/// \brief PY_StochasticDG class. Wrapper to PyTorch Stochastic Gradient Descent algorithm
template<typename ErrorFunction>
class PYT_StochasticGD: private boost::noncopyable
{

public:

    /// \brief The type used to measure the error
    typedef ErrorFunction error_t;

    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    typedef GDInfo output_t;

    /// \brief Constructor
    PYT_StochasticGD(const GDConfig& control);


    /// \brief Solves the optimization problem. Returns information
    /// about the performance of the solver.
    template<typename MatType, typename VecType, typename HypothesisFuncType>
    output_t solve(const MatType& mat,const VecType& v, HypothesisFuncType& h);


private:

    /// \brief Control of the algorithm
    GDConfig control_;

    /// \brief Error function used
    error_t error_func_;

};

template<typename ErrorFunction>
PYT_StochasticGD<ErrorFunction>::PYT_StochasticGD(const GDConfig& control)
    :
    control_(control),
    error_func_()
{}

template<typename ErrorFunction>
template<typename MatType, typename VecType, typename HypothesisFuncType>
typename PYT_StochasticGD<ErrorFunction>::output_t
PYT_StochasticGD<ErrorFunction>::solve(const MatType& data, const VecType& labels, HypothesisFuncType& h){

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //the info object to return
    GDInfo info;
    info.learning_rate = control_.learning_rate;


    // the coefficients of the hypothesis
    //auto& model = h.get_model();
    torch::optim::SGDOptions options(control_.learning_rate);
    torch::optim::SGD sgd(h->parameters(), options);

    std::cout << std::fixed << std::setprecision(4);

    while(control_.continue_iterations()){

        if(control_.show_iterations()){
            std::cout<<"PYT_SGD: iteration: "<<sgd.iteration()<<std::endl;
        }

        // Forward pass
        auto output = h(data);
        auto loss = error_func_(output, labels);

        // Clear gradient buffers because we don't want any
        // gradient from previous epoch to carry forward, dont want to cummulate gradients
        sgd.zero_grad();

        // Backward pass and optimize
        sgd.zero_grad();
        loss.backward();
        sgd.step();

        if (control_.show_iterations()) {
              std::cout <<"Loss: " << loss.item() <<std::endl;
        }
    }

    auto state = control_.get_state();

    end = std::chrono::system_clock::now();
    info.runtime = end-start;
    info.nprocs = 1;
    info.nthreads = 1;
    info.converged = state.converged;
    info.residual = state.residual;
    info.tolerance = state.tolerance;
    info.niterations = state.num_iterations;
    return info;
}


}

}
#endif
#endif // PYTORCH_STOCHASTIC_GRADIENT_DESCENT_H
