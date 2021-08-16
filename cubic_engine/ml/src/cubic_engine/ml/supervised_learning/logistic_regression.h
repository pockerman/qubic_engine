#ifndef LOGISTIC_REGRESSION_H
#define	LOGISTIC_REGRESSION_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/linear_parametric_model.h"
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

    ///
    /// \brief score
    ///
    template<typename DataSetTp>
    real_t score(const DataSetTp& data)const;

private:


    ///
    /// \brief transformer_
    ///
    kernel::SigmoidFunction<kernel::PolynomialFunction> transformer_;
 };

template<typename DataSetTp, typename SolverTp>
typename SolverTp::output_t
LogisticRegression::fit(const DataSetTp& dataset, SolverTp& solver, const std::map<std::string, std::any>& /*options*/){

#ifdef KERNEL_DEBUG
    assert(dataset.n_features() == polynomial_.n_coeffs() && "Invalid feature space size");
#endif

    //auto err_type = kernel::ErrorFuncType::MSE;
    //auto error_function_ptr = kernel::ErrFuncFactory().build<kernel::PolynomialFunction,
    //                                                         typename DataSetTp::features_t, typename DataSetTp::labels_t>(err_type, this->polynomial_);

    typename SolverTp::function_t loss(polynomial_);
    auto output = solver.solve(dataset, loss);

    return output;
}

template<typename DataSetTp>
real_t
LogisticRegression::score(const DataSetTp& dataset)const{

#ifdef KERNEL_DEBUG
    assert(dataset.n_features() == polynomial_.n_coeffs() && "Invalid feature space size");
    //check_options_(options);
#endif

    real_t correctly_clss = 0;
    for(uint_t p=0; p < dataset.size(); ++p){

        auto[point, label] = dataset[p];

        auto p_class = static_cast<uint_t>(predict_one(point));

        if(p_class == label){
            correctly_clss += 1.;
        }
    }

    return correctly_clss/dataset.size();

}
 

}
}

#endif

