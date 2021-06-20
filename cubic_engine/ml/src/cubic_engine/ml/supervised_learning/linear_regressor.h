#ifndef LINEAR_REGRESSOR_H
#define LINEAR_REGRESSOR_H

#include "cubic_engine/ml/supervised_learning/parametric_supervised_model.h"
#include "kernel/maths/vector_polynomial.h"

namespace cengine{
namespace ml {

///
/// \brief Forward declaration
///
class SupervisedDataset;

///
/// \brief The LinearRegressor class
///
class LinearRegressor: public ParametricSupervisedModel<SupervisedDataset, real_t>
{
public:

    ///
    /// \brief dataset_t. The type of the dataset
    ///
    typedef ParametricSupervisedModel<SupervisedDataset, real_t>::dataset_t dataset_t;

    ///
    /// \brief value_t The result value type
    ///
    typedef ParametricSupervisedModel<SupervisedDataset, real_t>::value_t value_t;


    ///
    /// \brief LinearRegressor
    ///
    LinearRegressor(uint_t n_features, bool use_intercept);

    ///
    /// \brief predict
    ///
    virtual value_t predict(const dataset_t& data)const;

    ///
    /// \brief predict
    ///
    virtual std::vector<value_t> predict_many(const dataset_t& data)const;

    ///
    /// \brief get_parameters. Returns the parameters of the model
    ///
    virtual std::vector<real_t> get_parameters()const{return polynomial_.get_parameters();}

    ///
    /// \brief update_parameters
    ///
    virtual void update_parameters(const std::vector<real_t>& parameters){polynomial_.set_coeffs(parameters);}

private:

    ///
    /// \brief polynomial_ The polynomial
    ///
    kernel::VectorPolynomial polynomial_;

};


}
}

#endif // LINEAR_REGRESSOR_H
