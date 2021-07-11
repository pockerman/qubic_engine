#ifndef LINEAR_REGRESSOR_H
#define LINEAR_REGRESSOR_H

#include "kernel/base/config.h"
#include "cubic_engine/ml/supervised_learning/parametric_supervised_model.h"
#include "cubic_engine/ml/supervised_learning/regularizer_type.h"
#include "kernel/maths/functions/vector_polynomial.h"

#include <ostream>
#include <map>
#include <any>

namespace cengine{
namespace ml {

///
/// \brief Forward declaration
///
class RegressionDataset;

///
/// \brief The LinearRegressor class
///
class LinearRegressor: public ParametricSupervisedModel<real_t>
{
public:

    ///
    /// \brief dataset_t. The type of the dataset
    ///
    typedef ParametricSupervisedModel<real_t>::dataset_t dataset_t;

    ///
    /// \brief value_t The result value type
    ///
    typedef ParametricSupervisedModel<real_t>::value_t value_t;

    ///
    /// \brief LinearRegressor
    ///
    LinearRegressor(uint_t n_features, bool use_intercept, RegularizerType rtype);

    ///
    /// \brief fit. Fit the model on the given dataset
    ///
    void fit(const dataset_t& dataset, const std::map<std::string, std::any>& options) override;

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
    /// \brief get_interception
    ///
    real_t get_interception()const;

    ///
    /// \brief update_parameters
    ///
    virtual void update_parameters(const std::vector<real_t>& parameters){polynomial_.set_coeffs(parameters);}

    ///
    /// \brief Print the model coeffs
    ///
    std::ostream& print(std::ostream& out)const;

    ///
    /// \brief predict
    ///
    virtual value_t predict(const DatasetBase& data)const override;

    ///
    /// \brief predict
    ///
    virtual std::vector<value_t> predict_many(const DatasetBase& data)const override;



private:

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
    kernel::VectorPolynomial polynomial_;

#ifdef KERNEL_DEBUG
    ///
    /// \brief check_options
    /// \param options
    ///
    void check_options_(const std::map<std::string, std::any>& options)const;
#endif

};


}
}

#endif // LINEAR_REGRESSOR_H
