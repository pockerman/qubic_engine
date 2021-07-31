#include "cubic_engine/ml/supervised_learning/logistic_regression.h"

namespace cengine{
namespace ml{



LogisticRegression::LogisticRegression(uint_t n_features, bool use_intercept)
:
  LinearParametricModel<uint_t>(n_features, use_intercept),
  transformer_(this->polynomial_)
{}


LogisticRegression::value_t
LogisticRegression::predict_one(const DynVec<real_t>& point)const{

    //TransformerType transformer(hypothesis_);
    auto value = transformer_.value(point);

    if(value >= 0.5){
        return 1;
    }

    return 0;
}

}
}
