#include "cubic_engine/ml/supervised_learning/linear_regressor.h"
#include "cubic_engine/ml/datasets/regression_dataset_base.h"
#include "kernel/base/config.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

namespace cengine{
namespace ml {

LinearRegressor::LinearRegressor(uint_t n_features, bool use_intercept,
                                 RegularizerType rtype)
    :
     ParametricSupervisedModel<real_t>(),
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

    // parameters vector
    DynVec<real_t> params(polynomial_.n_coeffs(), 0.0);

    // check if we

}

std::ostream&
LinearRegressor::print(std::ostream& out)const{

    return out;
}


LinearRegressor::value_t
LinearRegressor::predict(const DatasetBase& data)const{

    return 0.0;
}

std::vector<LinearRegressor::value_t >
LinearRegressor::predict_many(const DatasetBase& data)const{

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

}
#endif

}
}
