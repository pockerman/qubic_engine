#include "cubic_engine/ml/supervised_learning/linear_regressor.h"
#include "cubic_engine/ml/datasets/regression_dataset_base.h"
#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"

#include "kernel/base/config.h"
//#include "kernel/numerics/optimization/optimizer_type.h"
//#include "kernel/numerics/optimization/optimizer_factory.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

namespace cengine{
namespace ml {

LinearRegressor::LinearRegressor(uint_t n_features, bool use_intercept,
                                 RegularizerType rtype)
    :
     num_features_(n_features),
     use_intercept_(use_intercept),
     rtype_(rtype),
     polynomial_()
{

    num_features_ = use_intercept ? n_features + 1 : n_features;
    std::vector<int> order(num_features_, 1);
    order[0] = 0;
    polynomial_.create_from(DynVec<real_t>(num_features_, 0.0), order);
}

std::ostream&
LinearRegressor::print(std::ostream& out)const{

    for(uint_t c=0; c < polynomial_.n_coeffs(); ++c){
            out<<polynomial_.coeff(c);

            if(c == polynomial_.n_coeffs() - 1){
                out<<"\n";
            }
            else{
                out<<",";
            }
        }

        return out;
}


LinearRegressor::value_t
LinearRegressor::predict_one(const DynVec<real_t>& data)const{

#ifdef KERNEL_DEBUG
    assert(data.size() == num_features_ && "Invalid data point shape");
#endif

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
