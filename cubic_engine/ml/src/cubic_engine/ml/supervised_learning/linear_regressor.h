#ifndef LINEAR_REGRESSOR_H
#define LINEAR_REGRESSOR_H

#include "kernel/base/config.h"
#include "cubic_engine/ml/supervised_learning/parametric_supervised_model.h"
#include "cubic_engine/ml/supervised_learning/regularizer_type.h"
#include "kernel/maths/functions/real_vector_polynomial.h"
#include "kernel/maths/errorfunctions/error_function_type.h"
#include "kernel/maths/errorfunctions/error_function_factory.h"

#include <ostream>
#include <map>
#include <any>

namespace cengine{
namespace ml {


///
/// \brief The LinearRegressor class
///
class LinearRegressor
{
public:

    ///
    /// \brief value_t The result value type
    ///
    typedef real_t value_t;

    ///
    /// \brief LinearRegressor
    ///
    LinearRegressor(uint_t n_features, bool use_intercept, RegularizerType rtype);

    ///
    /// \brief fit. Fit the model on the given dataset
    ///
    template<typename DataSetTp, typename SolverTp>
    typename SolverTp::output_t fit(const DataSetTp& dataset, SolverTp& solver, const std::map<std::string, std::any>& options);

    ///
    /// \brief get_parameters. Returns the parameters of the model
    ///
    virtual DynVec<real_t> get_parameters()const{return polynomial_.coeffs();}

    ///
    /// \brief get_interception
    ///
    real_t get_interception()const;

    ///
    /// \brief update_parameters
    ///
    void update_parameters(const std::vector<real_t>& parameters){polynomial_.set_coeffs(parameters);}

    ///
    /// \brief Print the model coeffs
    ///
    std::ostream& print(std::ostream& out)const;

    ///
    /// \brief predict
    ///
    value_t predict_one(const DynVec<real_t>& data)const;

    ///
    /// \brief predict
    ///
    std::vector<value_t> predict_many(const DynMat<real_t>& data)const;

private:

    ///
    /// \brief num_features_
    ///
    uint_t num_features_;

    ///
    /// \brief use_intercept_
    ///
    bool use_intercept_;

    ///
    /// \brief rtype_
    ///
    RegularizerType rtype_;

    ///
    /// \brief polynomial_ The polynomial
    ///
    kernel::PolynomialFunction polynomial_;

#ifdef KERNEL_DEBUG
    ///
    /// \brief check_options
    /// \param options
    ///
    void check_options_(const std::map<std::string, std::any>& options)const;
#endif

};

template<typename DataSetTp, typename SolverTp>
typename SolverTp::output_t
LinearRegressor::fit(const DataSetTp& dataset, SolverTp& solver, const std::map<std::string, std::any>& options){

#ifdef KERNEL_DEBUG
    assert(dataset.n_features() == polynomial_.n_coeffs() && "Invalid feature space size");
    check_options_(options);
#endif

    auto err_type = std::any_cast<kernel::ErrorFuncType>(options.find("error_function_type")->second);
    auto error_function_ptr = kernel::ErrFuncFactory().build<kernel::PolynomialFunction, typename DataSetTp::features_t, typename DataSetTp::labels_t>(err_type, polynomial_);
    auto output = solver.solve(dataset.feature_matrix(), dataset.labels(), *error_function_ptr.get());

    return output;
}


}
}

#endif // LINEAR_REGRESSOR_H
