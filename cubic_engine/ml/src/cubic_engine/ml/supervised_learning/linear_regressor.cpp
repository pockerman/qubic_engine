#include "cubic_engine/ml/supervised_learning/linear_regressor.h"
#include "cubic_engine/ml/datasets/regression_dataset_base.h"
#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"

#include "kernel/base/config.h"
#include "kernel/maths/errorfunctions/error_function_type.h"
#include "kernel/maths/errorfunctions/error_function_factory.h"
#include "kernel/numerics/optimization/optimizer_type.h"
#include "kernel/numerics/optimization/optimizer_factory.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

namespace cengine{
namespace ml {

LinearRegressor::LinearRegressor(uint_t n_features, bool use_intercept,
                                 RegularizerType rtype)
    :
     use_intercept_(use_intercept),
     rtype_(rtype),
     polynomial_()
{

    auto num_features = use_intercept ? n_features + 1 : n_features;
    polynomial_.set_coeffs(std::vector<real_t>(num_features, 0.0));
}

void
LinearRegressor::fit(const dataset_t& dataset, const std::map<std::string, std::any>& options){

#ifdef KERNEL_DEBUG
    assert(dataset.n_features() == polynomial_.n_coeffs() && "Invalid feature space size");
    check_options_(options);
#endif

    auto opt_type =  std::any_cast<kernel::numerics::opt::OptimizerType>(options.find("solver_type")->second);

    const auto& solver_options = std::any_cast<const std::map<std::string, std::any>&>(options.find("solver_options")->second);
    auto solver = kernel::numerics::opt::OptimizerFactory().build<dataset_t::features_t, dataset_t::labels_t>(opt_type, solver_options);

    auto err_type = std::any_cast<kernel::ErrorFuncType>(options.find("error_function_type")->second);
    auto error_function_ptr = kernel::ErrFuncFactory().build<kernel::RealVectorPolynomialFunction, dataset_t::features_t, dataset_t::labels_t>(err_type, polynomial_);

    solver->solve(dataset.feature_matrix(), dataset.labels(), *error_function_ptr.get());
}

std::ostream&
LinearRegressor::print(std::ostream& out)const{

    return out;
}


LinearRegressor::value_t
LinearRegressor::predict_one(const DynVec<real_t>& data)const{
    return polynomial_.value(data);
}

std::vector<LinearRegressor::value_t >
LinearRegressor::predict_many(const DynMat<real_t>& data)const{

    return std::vector<LinearRegressor::value_t >();
}

real_t
LinearRegressor::get_interception()const{

#ifdef KERNEL_DEBUG
     assert(use_intercept_ && "No intercept term defined");
#endif

     return polynomial_.coeff(0);
}

#ifdef KERNEL_DEBUG

void
LinearRegressor::check_options_(const std::map<std::string, std::any>& options)const{

    if(options.empty()){
        assert(false && "Options are empty");
    }

    auto itr = options.find("solver_type");
    assert(itr != options.end() && "Solver was not specified");

    itr = options.find("solver_options");
    assert(itr != options.end() && "Solver options not specified");

    itr = options.find("error_function_type");
    assert(itr != options.end() && "Error metric was not specified");


}
#endif

}
}
