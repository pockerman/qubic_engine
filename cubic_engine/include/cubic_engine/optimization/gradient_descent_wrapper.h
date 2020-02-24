#ifndef BATCH_GRADIENT_DESCENT_WRAPPER_H
#define BATCH_GRADIENT_DESCENT_WRAPPER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/optimization/utils/gd_control.h"
#include "cubic_engine/optimization/utils/gd_info.h"
#include "cubic_engine/optimization/serial_gradient_descent.h"
#include "cubic_engine/optimization/threaded_gradient_descent.h"

#include <boost/noncopyable.hpp>

namespace cengine
{

template<typename ErrorFunction, typename ExecutorType, typename OptionsType>
class GradientDescentWrapper: private boost::noncopyable
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
    GradientDescentWrapper(const GDControl& input,
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
GradientDescentWrapper<ErrorFunction, ExecutorType,
                            OptionsType>::GradientDescentWrapper(const GDControl& input,
                                                                      typename GradientDescentWrapper<ErrorFunction, ExecutorType, OptionsType>::executor_t& executor,
                                                                      const  typename GradientDescentWrapper<ErrorFunction, ExecutorType, OptionsType>::options_t& options)
    :
      input_(input),
      executor_(executor),
      options_(options)
{}


template<typename ErrorFunction, typename ExecutorType, typename OptionsType>
template<typename MatType, typename VecType, typename HypothesisFuncType>
GDInfo
GradientDescentWrapper<ErrorFunction, ExecutorType, OptionsType>::solve(const MatType& mat,const VecType& v, HypothesisFuncType& h){

    ThreadedGd<ErrorFunction> gd(input_);
    return gd.solve(mat, v, h, executor_, options_);
}

template<typename ErrorFunction, typename ExecutorType, typename OptionsType>
template<typename MatType, typename VecType,
         typename HypothesisFuncType, typename RegularizerFuncType>
GDInfo
GradientDescentWrapper<ErrorFunction, ExecutorType, OptionsType>::solve(const MatType& mat, const VecType& v,
                                                                             HypothesisFuncType& h, const RegularizerFuncType& regularizer){

    ThreadedGd<ErrorFunction> gd(input_);
    return gd.solve(mat, v, h, regularizer, executor_, options_);
}

template<typename ErrorFunction, typename ExecutorType, typename OptionsType>
void
GradientDescentWrapper<ErrorFunction, ExecutorType, OptionsType>::reset_control(const GDControl& control){
    input_.reset(control);
}


/// \brief Specialization for serial. Do I really need this?
template<typename ErrorFunction>
class GradientDescentWrapper<ErrorFunction, Null, Null>: private boost::noncopyable
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
    GradientDescentWrapper(const GDControl& input,
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
GradientDescentWrapper<ErrorFunction, Null, Null>::GradientDescentWrapper(const GDControl& input,
                                                                      typename GradientDescentWrapper<ErrorFunction, Null, Null>::executor_t& executor,
                                                                      const  typename GradientDescentWrapper<ErrorFunction, Null, Null>::options_t& options)
    :
      input_(input),
      executor_(executor),
      options_(options)
{}


template<typename ErrorFunction>
template<typename MatType, typename VecType, typename HypothesisFuncType>
GDInfo
GradientDescentWrapper<ErrorFunction, Null, Null>::solve(const MatType& mat,const VecType& v, HypothesisFuncType& h){

    Gd<ErrorFunction> gd(input_);
    return gd.solve(mat, v, h);
}

template<typename ErrorFunction>
template<typename MatType, typename VecType,
         typename HypothesisFuncType, typename RegularizerFuncType>
GDInfo
GradientDescentWrapper<ErrorFunction, Null, Null>::solve(const MatType& mat,const VecType& v,
                                                              HypothesisFuncType& h, const RegularizerFuncType& regularizer){

    Gd<ErrorFunction> gd(input_);
    return gd.solve(mat, v, h, regularizer);
}

template<typename ErrorFunction>
void
GradientDescentWrapper<ErrorFunction, Null, Null>::reset_control(const GDControl& control){
    input_.reset(control);
}

}

#endif // BATCH_GRADIENT_DESCENT_WRAPPER_H
