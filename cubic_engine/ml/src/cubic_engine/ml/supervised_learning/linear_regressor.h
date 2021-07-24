#ifndef LINEAR_REGRESSOR_H
#define LINEAR_REGRESSOR_H

#include "kernel/base/config.h"
#include "cubic_engine/ml/supervised_learning/parametric_supervised_model.h"
#include "cubic_engine/ml/supervised_learning/regularizer_type.h"
#include "kernel/maths/functions/real_vector_polynomial.h"

#include <ostream>
#include <map>
#include <any>

namespace cengine{
namespace ml {

class BlazeRegressionDataset;


///
/// \brief The LinearRegressor class
///
class LinearRegressor
{
public:

    ///
    /// \brief dataset_t. The type of the dataset
    ///
    typedef BlazeRegressionDataset dataset_t;

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
    void fit(const dataset_t& dataset, const std::map<std::string, std::any>& options);

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
    kernel::RealVectorPolynomialFunction polynomial_;

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
