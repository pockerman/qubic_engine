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

template<typename ErrorFunction, typename ExecutorType, typename OptionsType>
class BatchGradientDescentWrapper: private boost::noncopyable
{
public:

    /// \brief The type used to measure the error
    typedef ErrorFunction error_t;

    /// \brief The type of the executor
    typedef ExecutorType executor_t;

    /// \brief The options to use
    typedef OptionsType options_t;

    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    typedef GDInfo output_t;

    /// \brief Constructor
    BatchGradientDescentWrapper(const GDControl& input,
                                executor_t& executor,
                                const options_t& options);

    /// \brief Solve the problem
    template<typename MatType, typename VecType, typename HypothesisFuncType>
    GDInfo solve(const MatType& mat, const VecType& v, HypothesisFuncType& h);

    /// \brief Solve the problem
    template<typename MatType, typename VecType,
             typename HypothesisFuncType, typename RegularizerFuncType>
    GDInfo solve(const MatType& mat, const VecType& v,
                 HypothesisFuncType& h, const RegularizerFuncType& regularizer);

    /// \brief Reset the control
    void reset_control(const GDControl& control);

private:

    /// \brief The control data the GD solver is using
    GDControl input_;

    /// \brief The executor used
    executor_t& executor_;

    /// \brief options for the executor
    const options_t& options_;
};

template<typename ErrorFunction, typename ExecutorType, typename OptionsType>
BatchGradientDescentWrapper<ErrorFunction, ExecutorType,
                            OptionsType>::BatchGradientDescentWrapper(const GDControl& input,
                                                                      typename BatchGradientDescentWrapper<ErrorFunction, ExecutorType, OptionsType>::executor_t& executor,
                                                                      const  typename BatchGradientDescentWrapper<ErrorFunction, ExecutorType, OptionsType>::options_t& options)
    :
      input_(input),
      executor_(executor),
      options_(options)
{}


template<typename ErrorFunction, typename ExecutorType, typename OptionsType>
template<typename MatType, typename VecType, typename HypothesisFuncType>
GDInfo
BatchGradientDescentWrapper<ErrorFunction, ExecutorType, OptionsType>::solve(const MatType& mat,const VecType& v, HypothesisFuncType& h){

    ThreadedGd<ErrorFunction> gd(input_);
    return gd.solve(mat, v, h, executor_, options_);
}

template<typename ErrorFunction, typename ExecutorType, typename OptionsType>
template<typename MatType, typename VecType,
         typename HypothesisFuncType, typename RegularizerFuncType>
GDInfo
BatchGradientDescentWrapper<ErrorFunction, ExecutorType, OptionsType>::solve(const MatType& mat, const VecType& v,
                                                                             HypothesisFuncType& h, const RegularizerFuncType& regularizer){

    ThreadedGd<ErrorFunction> gd(input_);
    return gd.solve(mat, v, h, regularizer, executor_, options_);
}

template<typename ErrorFunction, typename ExecutorType, typename OptionsType>
void
BatchGradientDescentWrapper<ErrorFunction, ExecutorType, OptionsType>::reset_control(const GDControl& control){
    input_.reset(control);
}


/// \brief Specialization for serial. Do I really need this?
template<typename ErrorFunction>
class BatchGradientDescentWrapper<ErrorFunction, Null, Null>: private boost::noncopyable
{
public:

    /// \brief The type used to measure the error
    typedef ErrorFunction error_t;

    /// \brief The type of the executor
    typedef Null executor_t;

    /// \brief The options to use
    typedef Null options_t;

    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    typedef GDInfo output_t;

    /// \brief Constructor
    BatchGradientDescentWrapper(const GDControl& input,
                                executor_t& executor,
                                const options_t& options);

    template<typename MatType, typename VecType, typename HypothesisFuncType>
    GDInfo solve(const MatType& mat, const VecType& v, HypothesisFuncType& h);

    /// \brief Solve the problem
    template<typename MatType, typename VecType,
             typename HypothesisFuncType, typename RegularizerFuncType>
    GDInfo solve(const MatType& mat, const VecType& v,
                 HypothesisFuncType& h, const RegularizerFuncType& regularizer);

    /// \brief Reset the control
    void reset_control(const GDControl& control);

private:

    /// \brief The control data the GD solver is using
    GDControl input_;

    /// \brief The executor used
    executor_t& executor_;

    /// \brief options for the executor
    const options_t& options_;
};

template<typename ErrorFunction>
BatchGradientDescentWrapper<ErrorFunction, Null, Null>::BatchGradientDescentWrapper(const GDControl& input,
                                                                      typename BatchGradientDescentWrapper<ErrorFunction, Null, Null>::executor_t& executor,
                                                                      const  typename BatchGradientDescentWrapper<ErrorFunction, Null, Null>::options_t& options)
    :
      input_(input),
      executor_(executor),
      options_(options)
{}


template<typename ErrorFunction>
template<typename MatType, typename VecType, typename HypothesisFuncType>
GDInfo
BatchGradientDescentWrapper<ErrorFunction, Null, Null>::solve(const MatType& mat,const VecType& v, HypothesisFuncType& h){

    Gd<ErrorFunction> gd(input_);
    return gd.solve(mat, v, h);
}

template<typename ErrorFunction>
template<typename MatType, typename VecType,
         typename HypothesisFuncType, typename RegularizerFuncType>
GDInfo
BatchGradientDescentWrapper<ErrorFunction, Null, Null>::solve(const MatType& mat,const VecType& v,
                                                              HypothesisFuncType& h, const RegularizerFuncType& regularizer){

    Gd<ErrorFunction> gd(input_);
    return gd.solve(mat, v, h, regularizer);
}

template<typename ErrorFunction>
void
BatchGradientDescentWrapper<ErrorFunction, Null, Null>::reset_control(const GDControl& control){
    input_.reset(control);
}

}

#endif // BATCH_GRADIENT_DESCENT_WRAPPER_H
