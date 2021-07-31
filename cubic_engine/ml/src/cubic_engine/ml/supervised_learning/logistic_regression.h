#ifndef LOGISTIC_REGRESSION_H
#define	LOGISTIC_REGRESSION_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/linear_parametric_model.h"
#include "kernel/maths/errorfunctions/error_function_type.h"
#include "kernel/maths/errorfunctions/error_function_factory.h"
#include "kernel/maths/functions/sigmoid_function.h"

#include <boost/noncopyable.hpp>
#include <vector>
#include <ostream>
#include <map>
#include <any>
#include <string>


namespace cengine{
namespace ml{


///
/// \brief LogisticRegression. Logistic Regression classifier suitable
/// only for binary class classification
///
class LogisticRegression: public  LinearParametricModel<uint_t>
{

 public:

    ///
    /// \brief output_t
    ///
    typedef LinearParametricModel<uint_t>::value_t value_t;

    ///
    /// \brief Constructor
    ///
    LogisticRegression(uint_t n_features, bool use_intercept);

    ///
    /// \brief fit. Fit the model on the given dataset
    ///
    template<typename DataSetTp, typename SolverTp>
    typename SolverTp::output_t fit(const DataSetTp& dataset, SolverTp& solver, const std::map<std::string, std::any>& options);

    ///
    /// \brief predict
    ///
    value_t predict_one(const DynVec<real_t>& data)const;

    ///
    /// \brief predict
    ///
    std::vector<value_t> predict_many(const DynMat<real_t>& data)const{}

private:


    ///
    /// \brief transformer_
    ///
    kernel::SigmoidFunction<kernel::PolynomialFunction> transformer_;
 };

template<typename DataSetTp, typename SolverTp>
typename SolverTp::output_t
LogisticRegression::fit(const DataSetTp& dataset, SolverTp& solver, const std::map<std::string, std::any>& options){

#ifdef KERNEL_DEBUG
    assert(dataset.n_features() == polynomial_.n_coeffs() && "Invalid feature space size");
    //check_options_(options);
#endif

    auto err_type = kernel::ErrorFuncType::MSE;
    auto error_function_ptr = kernel::ErrFuncFactory().build<kernel::PolynomialFunction,
                                                             typename DataSetTp::features_t, typename DataSetTp::labels_t>(err_type, this->polynomial_);
    auto output = solver.solve(dataset.feature_matrix(), dataset.labels(), *error_function_ptr.get());

    return output;
}
 

}
}

#endif

