#ifndef STOCHASTIC_GRADIENT_DESCENT_H
#define STOCHASTIC_GRADIENT_DESCENT_H

#include "kernel/base/types.h"
#include "kernel/maths/optimization/utils/gd_control.h"
#include "kernel/maths/optimization/utils/gd_info.h"
#include "kernel/maths/matrix_traits.h"

#include <boost/noncopyable.hpp>
#include <chrono>
#include <iostream>
#include <vector>

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
    template<typename MatTp, typename VecTp, typename FunctionTp>
    GDInfo solve(const MatTp& mat,const VecTp& v, FunctionTp& h)const;

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
SGD::do_solve_(const MatTp& mat,const VecTp& v,
               const ErrFunctionTp& error_metric, FunctionTp& h)const{


    auto total_error = error_metric.total_error();
    auto previous_error = error_metric.max_value();

    while(config_.continue_iterations()){

        total_error = error_metric.total_error();
        for(uint_t exidx = 0; exidx < mat.rows(); ++ exidx){

            auto row = matrix_row_trait<MatTp>::get_row(mat, exidx);

            auto err = v[exidx] - h.predict(row);

            // get the gradients with respect to the coefficients
            auto j_grad = error_metric.gradient(row, v[exidx]);

            auto coeffs = h.coeffs();

            for(uint_t c=0; c<coeffs.size(); ++c){
               coeffs[c] += config_.learning_rate*j_grad[c];
            }

            // reset again the coeffs
            h.set_coeffs(coeffs);
        }

        real_t error = std::fabs(previous_error - total_error);
        config_.update_residual(error);
        uint_t itr = config_.get_current_iteration();
        previous_error = total_error;


        //update the coefficients
        /*auto coeffs = h.coeffs();

        for(uint_t c=0; c<ncoeffs; ++c){
            coeffs[c] -= input_.learning_rate*j_grads[c];
        }

        // reset again the coeffs
        h.set_coeffs(coeffs);

        //recalculate...
        j_current = err_function_.value(data, y).get_resource();

        real_t error = std::fabs(j_current - j_old);
        input_.update_residual(error);
        uint_t itr = input_.get_current_iteration();

        if(input_.show_iterations()){

            std::cout<<"SGD: iteration: "<<itr<<std::endl;
            std::cout<<"\tJold: "<<j_old<<" Jcur: "<<j_current
                     <<" error std::fabs(Jcur-Jold): "<<error
                     <<" exit tolerance: "<<input_.get_exit_tolerance()<<std::endl;
        }

        j_old = j_current;*/

    }//itrs
}

}
}

}

#endif // STOCHASTIC_GRADIENT_DESCENT_H
