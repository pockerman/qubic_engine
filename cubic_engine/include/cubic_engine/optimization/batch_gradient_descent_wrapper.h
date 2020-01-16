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

    typedef ExecutorType executor_type;
    typedef OptionsType options_type;

    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    typedef GDInfo output_type;

    /// \brief Constructor
    BatchGradientDescentWrapper(const GDControl<ErrorFunction>& input,
                                executor_type& executor,
                                const options_type& options);

    template<typename MatType, typename VecType, typename HypothesisFuncType>
    GDInfo solve(const MatType& mat, const VecType& v, HypothesisFuncType& h);

    /// \brief Reset the control
    void reset_control(const GDControl<ErrorFunction>& control){input_ = control;}

private:

    /// \brief The control data the GD solver is using
    GDControl<ErrorFunction> input_;

    /// \brief The executor used
    executor_type& executor_;

    /// \brief options for the executor
    const options_type& options_;
};

template<typename ErrorFunction, typename ExecutorType, typename OptionsType>
BatchGradientDescentWrapper<ErrorFunction, ExecutorType,
                            OptionsType>::BatchGradientDescentWrapper(const GDControl<ErrorFunction>& input,
                                                                      typename BatchGradientDescentWrapper<ErrorFunction, ExecutorType, OptionsType>::executor_type& executor,
                                                                      const  typename BatchGradientDescentWrapper<ErrorFunction, ExecutorType, OptionsType>::options_type& options)
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


/// \brief Specialization for serial. Do I really need this?
template<typename ErrorFunction>
class BatchGradientDescentWrapper<ErrorFunction, Null, Null>: private boost::noncopyable
{
public:

    typedef Null executor_type;
    typedef Null options_type;

    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    typedef GDInfo output_type;

    /// \brief Constructor
    BatchGradientDescentWrapper(const GDControl<ErrorFunction>& input,
                                executor_type& executor,
                                const options_type& options);

    template<typename MatType, typename VecType, typename HypothesisFuncType>
    GDInfo solve(const MatType& mat, const VecType& v, HypothesisFuncType& h);

    /// \brief Reset the control
    void reset_control(const GDControl<ErrorFunction>& control){input_ = control;}

private:

    /// \brief The control data the GD solver is using
    GDControl<ErrorFunction> input_;

    /// \brief The executor used
    executor_type& executor_;

    /// \brief options for the executor
    const options_type& options_;
};

template<typename ErrorFunction>
BatchGradientDescentWrapper<ErrorFunction, Null, Null>::BatchGradientDescentWrapper(const GDControl<ErrorFunction>& input,
                                                                      typename BatchGradientDescentWrapper<ErrorFunction, Null, Null>::executor_type& executor,
                                                                      const  typename BatchGradientDescentWrapper<ErrorFunction, Null, Null>::options_type& options)
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

}

#endif // BATCH_GRADIENT_DESCENT_WRAPPER_H
