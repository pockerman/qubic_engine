#ifndef SERIAL_BATCH_GRADIENT_DESCENT_H
#define	SERIAL_BATCH_GRADIENT_DESCENT_H

#include "kernel/base/types.h"

#include "kernel/numerics/optimization/gd_control.h"
#include "kernel/numerics/optimization/optimizer_base.h"
#include "kernel/numerics/optimization/optimizer_type.h"
#include "kernel/maths/functions/function_base.h"

#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <any>

namespace kernel{
namespace numerics {
namespace opt {

///
/// \brief Implementation of the gradient descent (GC) algorithm
/// for solving optimization problems.
///
template<typename DatasetTp, typename FunctionTp>
class Gd: public OptimizerBase<DatasetTp, FunctionTp>
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
    Gd(const GDConfig& input);

    ///
    /// \brief Gd
    /// \param options
    ///
    Gd(const std::map<std::string, std::any>& options);
    
    ///
    /// \brief Solves the optimization problem. Returns information
    /// about the performance of the solver.
    ///
    virtual output_t solve(const data_set_t& mat, function_t& h) override final;

    ///
    /// \brief Solve for the coefficients that minimize
    /// the given function
    ///
    //template<typename FunctionTp>
    //GDInfo solve(FunctionTp& function);

    ///
    /// \brief Reset the control
    ///
    void reset_configuration(const GDConfig& control);

    ///
    /// \brief type
    /// \return
    ///
    virtual OptimizerType type()const override final{return OptimizerType::GD;}
      
private:
    
    ///
    /// \brief The control data the GD solver is using
    ///
    GDConfig input_;

    ///
    /// \brief info_
    ///
    output_t info_;

    ///
    /// \brief actually solve the problem
    ///
    output_t do_solve_(const data_set_t& mat, function_t& h);
    
};

template<typename DatasetTp, typename FunctionTp>
Gd<DatasetTp, FunctionTp>::Gd(const GDConfig& input)
    :
      OptimizerBase<DatasetTp, FunctionTp>(),
      input_(input),
      info_()
{}

template<typename DatasetTp, typename FunctionTp>
Gd<DatasetTp, FunctionTp>::Gd(const std::map<std::string, std::any>& options)
    :
      OptimizerBase<DatasetTp, FunctionTp>(),
      input_(options),
      info_()
{}

template<typename DatasetTp, typename FunctionTp>
typename Gd<DatasetTp, FunctionTp>::output_t
Gd<DatasetTp, FunctionTp>::solve(const data_set_t& data, function_t& function){
    info_ = do_solve_(data, function);
    return info_;
}


/*template<typename FunctionTp>
GDInfo
Gd::solve(FunctionTp& function){

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //the info object to return
    GDInfo info;
    info.learning_rate = input_.learning_rate;

    auto coeffs = function.coeffs();
    const uint_t ncoeffs = function.n_coeffs();
    auto val_old = function.value(coeffs);

    while(input_.continue_iterations()){

        auto grads = function.gradients(coeffs);

        for(uint_t c=0; c<ncoeffs; ++c){
            coeffs[c] -= input_.learning_rate*grads[c];
        }

        // reset again the coeffs
        function.set_coeffs(coeffs);

        auto new_val = function.value(function.coeffs());

        real_t error = std::fabs(new_val - val_old);
        input_.update_residual(error);
        uint_t itr = input_.get_current_iteration();

        if(input_.show_iterations()){

            std::cout<<">GD: iteration: "<<itr<<std::endl;
            std::cout<<"\t eta: "<<input_.learning_rate
                     <<" ABS error: "<<error
                     <<" Exit Tol: "<<input_.get_exit_tolerance()<<std::endl;
        }
        val_old = new_val;
    }

    auto state = input_.get_state();

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
*/

template<typename DatasetTp, typename FunctionTp>
typename Gd<DatasetTp, FunctionTp>::output_t
Gd<DatasetTp, FunctionTp>::do_solve_(const data_set_t& data, function_t& function){


    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();


    //the info object to return
    auto info = typename Gd<DatasetTp, FunctionTp>::output_t();

    if(input_.track_residuals()){
        info.residuals.reserve(input_.get_max_iterations());
    }

    auto j_old = function.evaluate(data);
    auto j_current = 0.0;

    const auto ncoeffs = function.n_parameters();

    while(input_.continue_iterations()){

        // get the gradients with respect to the coefficients
        auto j_grads = function.params_gradients(data);

        //update the coefficients
        auto coeffs = function.parameters();

        for(uint_t c=0; c<ncoeffs; ++c){
            coeffs[c] -= input_.learning_rate*j_grads[c];
        }

        // reset again the coeffs
        function.update_parameters(coeffs);

        //recalculate...
        j_current = function.evaluate(data);

        auto error = std::fabs(j_current - j_old);
        input_.update_residual(error);

        if(input_.track_residuals()){
            info.residuals.push_back(error);
        }

        auto itr = input_.get_current_iteration();

        if(input_.show_iterations()){

            std::cout<<"GD: iteration: "<<itr<<std::endl;
            std::cout<<"\t eta: "<<input_.learning_rate
                     <<" absolute error: "<<error
                     <<" exit tolerance: "<<input_.get_exit_tolerance()<<std::endl;
        }

        j_old = j_current;
    }

    auto state = input_.get_state();

    end = std::chrono::system_clock::now();
    info.total_time =  std::chrono::duration_cast<std::chrono::seconds>(end - start); //end-start;
    info.converged = state.converged;
    info.residual = state.residual;
    info.tolerance = state.tolerance;
    info.num_iterations = state.num_iterations;
    return info;
}

template<typename MatType, typename VecType>
void
Gd<MatType, VecType>::reset_configuration(const GDConfig& control){
    input_.reset(control);
    info_ = typename Gd<MatType, VecType>::output_t();
}
        
}
}
}

#endif	/* GRADIENT_DESCENT_H */

