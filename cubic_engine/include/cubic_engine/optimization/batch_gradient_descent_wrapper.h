#ifndef BATCH_GRADIENT_DESCENT_WRAPPER_H
#define BATCH_GRADIENT_DESCENT_WRAPPER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/optimization/utils/gd_control.h"
#include "cubic_engine/optimization/utils/gd_info.h"
#include "cubic_engine/optimization/serial_batch_gradient_descent.h"
#include "cubic_engine/optimization/threaded_batch_gradient_descent.h"

#include <boost/noncopyable.hpp>

namespace cengine
{

template<typename ExecutorType, typename OptionsType>
class BatchGradientDescentWrapper: private boost::noncopyable
{
public:

    typedef ExecutorType executor_type;
    typedef OptionsType options_type;

    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    typedef GDInfo output_type;

    /// \brief Constructor
    BatchGradientDescentWrapper(const GDControl& input,
                                executor_type& executor,
                                const options_type& options);

    template<typename MatType, typename VecType,
             typename ErrorFuncType, typename HypothesisFuncType>
    GDInfo solve(const MatType& mat, const VecType& v,
                 ErrorFuncType& error_fun, HypothesisFuncType& h);

    /// \brief Reset the control
    void reset_control(const GDControl& control){input_ = control;}

private:

    /// \brief The control data the GD solver is using
    GDControl input_;

    /// \brief The executor used
    executor_type& executor_;

    /// \brief options for the executor
    const options_type& options_;
};

template<typename ExecutorType,
         typename OptionsType>
BatchGradientDescentWrapper<ExecutorType,
                            OptionsType>::BatchGradientDescentWrapper(const GDControl& input,
                                                                     typename BatchGradientDescentWrapper<ExecutorType,
                                                                                                       OptionsType>::executor_type& executor,
                                                                      const  typename BatchGradientDescentWrapper<ExecutorType,
                                                                                                       OptionsType>::options_type& options)
    :
      input_(input),
      executor_(executor),
      options_(options)
{}


template<typename ExecutorType,
         typename OptionsType>
template<typename MatType, typename VecType,
         typename ErrorFuncType, typename HypothesisFuncType>
GDInfo
BatchGradientDescentWrapper<ExecutorType, OptionsType>::solve(const MatType& mat,const VecType& v,
                                                              ErrorFuncType& error_fun, HypothesisFuncType& h){

    ThreadedGd gd(input_);
    return gd.solve(mat, v, error_fun, h, executor_, options_);
}

template<>
template<typename MatType, typename VecType,
         typename ErrorFuncType, typename HypothesisFuncType>
GDInfo
BatchGradientDescentWrapper<Null, Null>::solve(const MatType& mat,const VecType& v,
                                         ErrorFuncType& error_fun, HypothesisFuncType& h){

    Gd gd(input_);
    return gd.solve(mat, v, error_fun, h);
}

}

#endif // BATCH_GRADIENT_DESCENT_WRAPPER_H
