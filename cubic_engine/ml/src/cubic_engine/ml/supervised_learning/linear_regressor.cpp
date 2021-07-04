#include "cubic_engine/ml/supervised_learning/linear_regressor.h"
#include "cubic_engine/ml/datasets/regression_dataset.h"
#include "kernel/base/config.h"

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
LinearRegressor::fit(const dataset_t& dataset){

#ifdef KERNEL_DEBUG
    assert(dataset.n_features() == polynomial_.n_coeffs() && "Invalid feature space size");
#endif

    // parameters vector
    DynVec<real_t> params(polynomial_.n_coeffs(), 0.0);

}

}
}
