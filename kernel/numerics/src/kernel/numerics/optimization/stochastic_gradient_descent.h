#ifndef STOCHASTIC_GRADIENT_DESCENT_H
#define STOCHASTIC_GRADIENT_DESCENT_H

#include "kernel/base/types.h"
#include "kernel/maths/matrix_traits.h"
#include "kernel/maths/functions/function_base.h"
#include "kernel/numerics/optimization/gd_control.h"
#include "kernel/numerics/optimization/optimizer_base.h"
#include "kernel/numerics/optimization/optimizer_type.h"

#include <map>
#include <string>
#include <any>
#include <chrono>
#include <iostream>
#include <vector>
#include <limits>

namespace kernel {
namespace numerics{
namespace opt {

///
/// \brief The SGD class. Vanilla implementation of stochastic gradient descent
/// optimization algorithm
///
template<typename DatasetTp, typename FunctionTp>
class SGD: public OptimizerBase<DatasetTp, FunctionTp>
{
public:

    ///
    /// \brief data_set_t
    ///
    typedef typename OptimizerBase<DatasetTp, FunctionTp>::data_set_t data_set_t;

    ///
    /// \brief function_t
    ///
    typedef typename OptimizerBase<DatasetTp, FunctionTp>::function_t function_t;

    ///
    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    ///
    typedef typename OptimizerBase<DatasetTp, FunctionTp>::output_t output_t;

    ///
    /// \brief Constructor
    ///
    SGD(const GDConfig& input);

    ///
    /// \brief Gd
    /// \param options
    ///
    SGD(const std::map<std::string, std::any>& options);

    ///
    /// \brief Solves the optimization problem. Returns information
    /// about the performance of the solver.
    ///
    virtual output_t solve(const data_set_t& mat, function_t& h) override final;

    ///
    /// \brief type
    /// \return
    ///
    virtual OptimizerType type()const override final{return OptimizerType::SGD;}

    ///
    /// \brief Reset the control
    ///
    void reset_configuration(const GDConfig& control){input_.reset(control);}

private:

    ///
    /// \brief config_. Configuration of the algorithm
    ///
    mutable GDConfig input_;

    ///
    /// \brief do_solve_
    /// \param mat
    /// \param v
    /// \param h
    /// \return
    ///
    output_t do_solve_(const data_set_t& mat, function_t& h);
};

template<typename DatasetTp, typename FunctionTp>
SGD<DatasetTp, FunctionTp>::SGD(const GDConfig& input)
    :
      OptimizerBase<DatasetTp, FunctionTp>(),
      input_(input)
{}

template<typename DatasetTp, typename FunctionTp>
SGD<DatasetTp, FunctionTp>::SGD(const std::map<std::string, std::any>& options)
    :
      OptimizerBase<DatasetTp, FunctionTp>(),
      input_(options)
{}

template<typename DatasetTp, typename FunctionTp>
typename SGD<DatasetTp, FunctionTp>::output_t
SGD<DatasetTp, FunctionTp>::solve(const data_set_t& mat, function_t& h){
    return do_solve_(mat, h);
}

template<typename DatasetTp, typename FunctionTp>
typename SGD<DatasetTp, FunctionTp>::output_t
SGD<DatasetTp, FunctionTp>::do_solve_(const data_set_t& mat, function_t& h){

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //the info object to return
    auto info = typename SGD<DatasetTp, FunctionTp>::output_t();

    if(input_.track_residuals()){
        info.residuals.reserve(input_.get_max_iterations());
    }

    auto previous_error = std::numeric_limits<real_t>::min();

    while(input_.continue_iterations()){

        if(input_.show_iterations()){

            std::cout<<"SGD: iteration: "<<input_.get_current_iteration()
                     <<" eta: "<<input_.learning_rate<<std::endl;
        }

        // total error for iteration
        auto total_error = 0.0;
        for(uint_t exidx = 0; exidx < mat.n_examples(); ++ exidx){

            auto [row, label] = mat[exidx];
            auto error = h.error_at(row, label);

            // get the gradients with respect to the coefficients
            auto j_grad = h.param_gradient_at(row, label);
            auto coeffs = h.parameters();

            for(uint_t c=0; c<coeffs.size(); ++c){
               coeffs[c] += -input_.learning_rate*j_grad[c];
            }

            // reset again the coeffs
            h.update_parameters(coeffs);
            total_error += error;
        }

        real_t error = std::fabs(previous_error - total_error);
        input_.update_residual(error);

        if(input_.track_residuals()){
            info.residuals.push_back(error);
        }

        previous_error = total_error;

        uint_t itr = input_.get_current_iteration();
        if(input_.show_iterations()){

            std::cout<<"\tAbsolute Total Error: "<<error
                     <<" Tol: "<<input_.get_exit_tolerance()<<std::endl;
        }
    }//itrs

    auto state = input_.get_state();

    end = std::chrono::system_clock::now();
    info.total_time = std::chrono::duration_cast<std::chrono::seconds>(end - start); //end-start;
    info.converged = state.converged;
    info.residual = state.residual;
    info.tolerance = state.tolerance;
    info.num_iterations = state.num_iterations;

    return info;
}

}
}

}

#endif // STOCHASTIC_GRADIENT_DESCENT_H
