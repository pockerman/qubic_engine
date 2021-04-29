#ifndef STOCHASTIC_GRADIENT_DESCENT_H
#define STOCHASTIC_GRADIENT_DESCENT_H

#include "kernel/base/types.h"
#include "kernel/numerics/optimization/utils/gd_control.h"
#include "kernel/numerics/optimization/utils/gd_info.h"
#include "kernel/maths/matrix_traits.h"

#include <boost/noncopyable.hpp>
#include <chrono>
#include <iostream>
#include <vector>
#include <limits>

namespace kernel {
namespace maths{
namespace opt {

///
/// \brief The SGD class. Stochastic Gradient Descent
/// optimization algorithm
///
class SGD: private boost::noncopyable
{
public:

    ///
    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    ///
    typedef GDInfo output_t;

    ///
    /// \brief Constructor
    ///
    SGD(const GDConfig& input);

    ///
    /// \brief Solves the optimization problem. Returns information
    /// about the performance of the solver.
    ///
    template<typename MatTp, typename VecTp,
             typename ErrFunctionTp, typename FunctionTp>
    GDInfo solve(const MatTp& mat,const VecTp& v,
                 const ErrFunctionTp& error_metric, FunctionTp& h)const;

private:

    ///
    /// \brief config_. Configuration of the algorithm
    ///
    mutable GDConfig config_;


    template<typename MatTp, typename VecTp,
             typename ErrFunctionTp, typename FunctionTp>
    GDInfo do_solve_(const MatTp& mat,const VecTp& v,
                     const ErrFunctionTp& error_metric, FunctionTp& h)const;
};

SGD::SGD(const GDConfig& input)
    :
      config_(input)
{}


template<typename MatTp, typename VecTp,
         typename ErrFunctionTp, typename FunctionTp>
GDInfo
SGD::solve(const MatTp& mat,const VecTp& v,
           const ErrFunctionTp& error_metric, FunctionTp& h)const{
    return do_solve_(mat, v, error_metric, h);
}

template<typename MatTp, typename VecTp,
         typename ErrFunctionTp, typename FunctionTp>
GDInfo
SGD::do_solve_(const MatTp& mat,const VecTp& v,
               const ErrFunctionTp& error_metric, FunctionTp& h)const{

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    GDInfo result;
    result.learning_rate = config_.learning_rate;

    auto previous_error = std::numeric_limits<real_t>::min();

    while(config_.continue_iterations()){

        if(config_.show_iterations()){

            std::cout<<"SGD: iteration: "<<config_.get_current_iteration()
                     <<" eta: "<<config_.learning_rate<<std::endl;
        }

        // total error for iteration
        auto total_error = 0.0;
        for(uint_t exidx = 0; exidx < mat.rows(); ++ exidx){


            auto row = matrix_row_trait<MatTp>::get_row(mat, exidx);
            auto val = h.value(row);
            auto error = v[exidx] - val;

            // get the gradients with respect to the coefficients
            auto j_grad = error_metric.gradient(row, v[exidx]);
            auto coeffs = h.coeffs();

            for(uint_t c=0; c<coeffs.size(); ++c){
               coeffs[c] += -config_.learning_rate*j_grad[c];
            }

            // reset again the coeffs
            h.set_coeffs(coeffs);
            total_error += error;
        }

        real_t abs_error = std::fabs(previous_error - total_error);
        config_.update_residual(abs_error);
        previous_error = total_error;

        uint_t itr = config_.get_current_iteration();
        if(config_.show_iterations()){

            std::cout<<"\tAbsolute Total Error: "<<abs_error
                     <<" Tol: "<<config_.get_exit_tolerance()<<std::endl;
        }
    }//itrs

    auto state = config_.get_state();

    end = std::chrono::system_clock::now();
    result.runtime = end-start;
    result.nprocs = 1;
    result.nthreads = 1;
    result.converged = state.converged;
    result.residual = state.residual;
    result.tolerance = state.tolerance;
    result.niterations = state.num_iterations;

    return result;
}

}
}

}

#endif // STOCHASTIC_GRADIENT_DESCENT_H
