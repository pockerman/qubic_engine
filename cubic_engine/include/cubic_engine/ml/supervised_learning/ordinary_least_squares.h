#ifndef ORDINARY_LEAST_SQUARES_H
#define ORDINARY_LEAST_SQUARES_H

#include "cubic_engine/base/config.h"
#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/maths/functions/real_vector_polynomial.h"

#include <boost/noncopyable.hpp>

namespace  cengine{
namespace ml {

///
/// \brief The OrdinaryLeastSquares class. Ordinary least
/// squares implementation. Uses the normal equations
/// for establishing the coefficients
///
class OrdinaryLeastSquares: private boost::noncopyable
{
public:

    ///
    /// \brief OrdinaryLeastSquare. Constructor
    /// build a linear model having num_coeffs.
    /// This number excludes the interception term
    ///
    OrdinaryLeastSquares(uint_t num_coeffs, bool use_intercept=true);

    ///
    /// \brief Train the model
    ///
    void train(const DynMat<real_t>& dataset, const DynVec<real_t>& y);

    ///
    /// \brief predict Predict the output for vector x using
    /// the trained model
    ///
    real_t predict(const DynVec<real_t>& x)const;

    ///
    /// \brief get_interception
    ///
    real_t get_interception()const{return intercept_;}

private:

    ///
    /// \brief use_intercept_. Flag indicating if the interception
    /// term is used in the model
    ///
    bool use_intercept_;

    ///
    /// \brief intercept_ The intersecption term
    ///
    real_t intercept_;

    ///
    /// \brief hypothesis_ The polynomial to fit in the data set
    ///
    kernel::RealVectorPolynomialFunction hypothesis_;
};


}

}

#endif // ORDINARY_LEAST_SQUARES_H
