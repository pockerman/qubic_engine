#include "cubic_engine/ml/supervised_learning/ordinary_least_squares.h"

#include <vector>
#include <stdexcept>

namespace cengine{
namespace ml{

OrdinaryLeastSquares::OrdinaryLeastSquares(uint_t num_coeffs, bool use_intercept)
    :
     use_intercept_(use_intercept),
     intercept_(0.0),
     hypothesis_()
{
    std::vector<real_t> coeffs(num_coeffs, 0.0);
    hypothesis_.create_from(coeffs);
}

void
OrdinaryLeastSquares::train(const DynMat<real_t>& dataset, const DynVec<real_t>& y){

    // if using interception then the dataset
    // is expected to have an extra columns of ones
    auto num_coeffs = use_intercept_ ? hypothesis_.n_coeffs() + 1 : hypothesis_.n_coeffs();

    if(dataset.columns() != num_coeffs){
        throw std::logic_error("Incompatible data set format with model. Number of features "+
                               std::to_string(dataset.columns()) +
                               " not equal to number of model parameters "+
                               std::to_string(num_coeffs));
    }

    auto X_T = trans(dataset);
    auto invs = inv(X_T*dataset);
    DynVec<real_t> coeffs = invs*X_T*y;

    if(use_intercept_){

        DynVec<real_t> other_coeffs(hypothesis_.n_coeffs(), 0.0);
        for(uint_t i=0; i<other_coeffs.size(); ++i){
           other_coeffs[i] = coeffs[i+1];
        }

        hypothesis_.set_coeffs(other_coeffs);
    }
    else{
       hypothesis_.set_coeffs(coeffs);
    }
}

real_t
OrdinaryLeastSquares::predict(const DynVec<real_t>& x)const{

    auto prediction = hypothesis_.value(x);

    if(use_intercept_){
        prediction += intercept_;
    }

    return prediction;
}

}
}
