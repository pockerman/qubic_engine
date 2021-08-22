#include "cubic_engine/ml/supervised_learning/logistic_regression.h"
#include "kernel/maths/matrix_traits.h"

namespace cengine{
namespace ml{



LogisticRegression::LogisticRegression(uint_t n_features, bool use_intercept)
:
  LinearParametricModel<uint_t>(n_features, use_intercept),
  transformer_(this->polynomial_)
{}


LogisticRegression::value_t
LogisticRegression::predict_one(const DynVec<real_t>& point)const{

    auto value = transformer_.value(point);

    if(value >= 0.5){
        return 1;
    }

    return 0;
}

DynVec<LogisticRegression::value_t>
LogisticRegression::predict_many(const DynMat<real_t>& data)const{

    DynVec<LogisticRegression::value_t> predictions(data.rows());

    for(uint_t r = 0; r < data.rows(); ++r){

        auto example = kernel::matrix_row_trait<DynMat<real_t>>::get_row(data, r);
        predictions[r] = predict_one(example);
    }

    return predictions;
}

}
}
