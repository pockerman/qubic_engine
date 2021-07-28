#ifndef MSE_FUNCTION_H
#define MSE_FUNCTION_H

#include "kernel/base/types.h"
#include "kernel/maths/functions/function_base.h"
#include "kernel/maths/functions/dummy_function.h"
#include "kernel/maths/matrix_utilities.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/parallel/utilities/result_holder.h"
#include "kernel/parallel/utilities/partitioned_type.h"

#include <iostream>
#include <exception>
#include <vector>

namespace kernel
{

///forward declarations
template<typename HypothesisFn> class SigmoidFunction;

namespace detail
{

template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn=DummyFunction<real_t, DataSetType, LabelsType>>
class mse_detail: public FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>
{
public:

    typedef typename FunctionBase<ResultHolder<real_t>, DataSetType, LabelsType>::output_t output_t;
    typedef std::pair<DataSetType, LabelsType> input_t;
    typedef HypothesisFn hypothesis_t;
    typedef RegularizerFn regularizer_t;

    /// \brief Constructor
    mse_detail();

    /// \brief Constructor
    mse_detail(hypothesis_t& h);

    /// \brief Constructor
    mse_detail(hypothesis_t& h, const regularizer_t& r);

    /// \brief Destructor
    virtual ~ mse_detail(){}

    /// \brief Returns the value of the function
    virtual output_t value(const DataSetType& dataset, const LabelsType& labels)const override;

    /// \brief Returns the gradients of the function
    virtual DynVec<real_t> gradients(const DataSetType& dataset, const LabelsType& labels)const override;

    ///
    /// \brief Returns the number of coefficients
    ///
    virtual uint_t n_coeffs()const override final{return h_ptr_->n_coeffs();}

    ///
    /// \brief Set the hypothesis pointer
    ///
    void set_hypothesis_function(hypothesis_t& h){h_ptr_ = &h;}

    ///
    /// \brief Set the regularization pointer
    ///
    void set_regularizer_function(const regularizer_t& r){r_ptr_ = &r;}

    ///
    /// \brief Update the underlying model
    ///
    template<typename VectorContainerTp>
    void update_model(const VectorContainerTp& coeffs){h_ptr_->set_coeffs(coeffs);}

    ///
    /// \brief coeffs. Returns the coefficients of the underlying model
    ///
    DynVec<real_t> coeffs()const override final{return h_ptr_->coeffs();}

    ///
    /// \brief update_coeffs
    /// \param params
    ///
    virtual void update_coeffs(const DynVec<real_t>& params) override final{update_model(params);};


protected:

    hypothesis_t* h_ptr_;
    const regularizer_t* r_ptr_;

};

}

/// \brief brief The MSEFunction class. Models the Mean Squred Error
template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn=DummyFunction<real_t, DataSetType, LabelsType>>
class MSEFunction: public detail::mse_detail<HypothesisFn, DataSetType, LabelsType, RegularizerFn>
{
public:

    typedef typename detail::mse_detail<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::output_t output_t;
    typedef std::pair<DataSetType, LabelsType> input_t;
    typedef typename detail::mse_detail<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::hypothesis_t hypothesis_t;
    typedef typename detail::mse_detail<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::regularizer_t regularizer_t;

    /// \brief Constructor
    MSEFunction();

    /// \brief Constructor
    MSEFunction(hypothesis_t& h);

    /// \brief Constructor
    MSEFunction(hypothesis_t& h, const regularizer_t& r);
};

/// \brief MSE function when using sigmoid transformation
template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
class MSEFunction<SigmoidFunction<HypothesisFn>, DataSetType,
                  LabelsType, RegularizerFn>  : public detail::mse_detail<SigmoidFunction<HypothesisFn>,
                                                                          DataSetType,
                                                                          LabelsType,
                                                                         RegularizerFn>
{
public:

    typedef typename detail::mse_detail<SigmoidFunction<HypothesisFn>,
                                        DataSetType,
                                        LabelsType,
                                        RegularizerFn>::output_t output_t;

    typedef std::pair<DataSetType, LabelsType> input_t;

    typedef typename detail::mse_detail<SigmoidFunction<HypothesisFn>, DataSetType, LabelsType, RegularizerFn>::hypothesis_t hypothesis_t;
    typedef typename detail::mse_detail<SigmoidFunction<HypothesisFn>, DataSetType, LabelsType, RegularizerFn>::regularizer_t regularizer_t;

    ///
    /// \brief Constructor
    ///
    MSEFunction();

    ///
    /// \brief Constructor
    ///
    MSEFunction(hypothesis_t& h);

    ///
    /// \brief Constructor
    ///
    MSEFunction(hypothesis_t& h, const regularizer_t& r);

    ///
    /// \brief Returns the value of the function
    ///
    virtual output_t value(const DataSetType& dataset, const LabelsType& labels)const override final;

};

///
/// \brief he MSEFunction class. Models the Mean Squred Error
/// This is a partial specialization to account for partitioned datasets
///  in this case we also allow parallel execution if an executor is given
///
/*template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
class MSEFunction<HypothesisFn, PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>, RegularizerFn>: public detail::mse_detail<HypothesisFn, DataSetType, LabelsType, RegularizerFn>

{
public:

    typedef typename detail::mse_detail<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::output_t output_t;
    typedef std::pair<DataSetType, LabelsType> input_t;
    typedef typename detail::mse_detail<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::hypothesis_t hypothesis_t;
    typedef typename detail::mse_detail<HypothesisFn, DataSetType, LabelsType, RegularizerFn>::regularizer_t regularizer_t;

    /// \brief Constructor
    MSEFunction();

    /// \brief Constructor
    MSEFunction(hypothesis_t& h);

    /// \brief Constructor
    MSEFunction(hypothesis_t& h, const regularizer_t& r);


    /// \brief Returns the value of the function using the provided executor
    template<typename Executor, typename Options>
    output_t value(const PartitionedType<DataSetType>& dataset,
                   const PartitionedType<LabelsType>& labels,
                   Executor& executor, const Options& options);

    /// \brief Returns the gradients of the function with respect to the
    /// coefficients of the hypothesis function
    template<typename Executor, typename Options>
    ResultHolder<DynVec<real_t>> gradients(const PartitionedType<DataSetType>& dataset,
                                           const PartitionedType<LabelsType>& labels,
                                            Executor& executor, const Options& options);

private:


    template<typename Executor, typename Options>
    void execute_value_tasks_(const PartitionedType<DataSetType>& dataset,
                              const PartitionedType<LabelsType>& labels,
                               Executor& executor, const Options& options);

    template<typename Executor, typename Options>
    void execute_gardient_value_tasks_(const PartitionedType<DataSetType>& dataset,
                                       const PartitionedType<LabelsType>& labels,
                                       Executor& executor, const Options& options);

    /// \brief Struct describing the task
    /// of evaluating the MSE function
    struct task_value_description;
    typedef task_value_description task_value_type;

    struct task_gradient_value_description;
    typedef task_gradient_value_description task_gradient_type;

    /// \list of value tasks
    std::vector<std::unique_ptr<task_value_type>> value_tasks_;

    /// \list of gradient tasks
    std::vector<std::unique_ptr<task_gradient_type>> gradient_tasks_;

};*/



/// \brief he MSEFunction class. Models the Mean Squred Error
/// This is a partial specialization to account for partitioned datasets
/// in this case we also allow parallel execution if an executor is given
/// Furthermore this accounts for SigmoidFunction transformation
/*template<typename HypothesisFn, typename DataSetType,
         typename LabelsType, typename RegularizerFn>
class MSEFunction<SigmoidFunction<HypothesisFn>,
                  PartitionedType<DataSetType>,
                  PartitionedType<LabelsType>,
                  RegularizerFn>: public detail::mse_detail<SigmoidFunction<HypothesisFn>,
                                                            DataSetType,
                                                            LabelsType,
                                                            RegularizerFn>

{
public:

    typedef typename detail::mse_detail<SigmoidFunction<HypothesisFn>,
                                        DataSetType,
                                        LabelsType,
                                        RegularizerFn>::output_t output_t;

    typedef std::pair<DataSetType, LabelsType> input_t;
    typedef typename detail::mse_detail<SigmoidFunction<HypothesisFn>,
                                        DataSetType,
                                        LabelsType,
                                        RegularizerFn>::hypothesis_t hypothesis_t;

    typedef typename detail::mse_detail<SigmoidFunction<HypothesisFn>,
                                        DataSetType,
                                        LabelsType,
                                        RegularizerFn>::regularizer_t regularizer_t;

    /// \brief Constructor
    MSEFunction();

    /// \brief Constructor
    MSEFunction(hypothesis_t& h);

    /// \brief Constructor
    MSEFunction(hypothesis_t& h, const regularizer_t& r);


    /// \brief Returns the value of the function using the provided executor
    template<typename Executor, typename Options>
    output_t value(const PartitionedType<DataSetType>& dataset,
                   const PartitionedType<LabelsType>& labels,
                   Executor& executor, const Options& options);

    /// \brief Returns the gradients of the function with respect to the
    /// coefficients of the hypothesis function
    template<typename Executor, typename Options>
    ResultHolder<DynVec<real_t>> gradients(const PartitionedType<DataSetType>& dataset,
                                           const PartitionedType<LabelsType>& labels,
                                           Executor& executor, const Options& options);
private:


    template<typename Executor, typename Options>
    void execute_value_tasks_(const PartitionedType<DataSetType>& dataset,
                              const PartitionedType<LabelsType>& labels,
                               Executor& executor, const Options& options);

    template<typename Executor, typename Options>
    void execute_gardient_value_tasks_(const PartitionedType<DataSetType>& dataset,
                                       const PartitionedType<LabelsType>& labels,
                                       Executor& executor, const Options& options);

    /// \brief Struct describing the task
    /// of evaluating the MSE function
    struct task_value_description;
    typedef task_value_description task_value_type;

    struct task_gradient_value_description;
    typedef task_gradient_value_description task_gradient_type;

    /// \list of value tasks
    std::vector<std::unique_ptr<task_value_type>> value_tasks_;

    /// \list of gradient tasks
    std::vector<std::unique_ptr<task_gradient_type>> gradient_tasks_;

};*/

}

#include "kernel/maths/errorfunctions/mse_function_impl.h"

#endif // MSE_FUNCTION_H
