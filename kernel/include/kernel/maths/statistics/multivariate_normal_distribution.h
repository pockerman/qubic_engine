#ifndef MULTIVARIATE_NORMAL_DISTRIBUTION_H
#define MULTIVARIATE_NORMAL_DISTRIBUTION_H

#include "kernel/base/types.h"

namespace kernel {
namespace maths {
namespace stats {


/// \brief Simple implementation
/// of multivariate normal distribution

class MultiNormalDist
{

public:

    /// Constructor. Construct with
    /// mu = [0,0,...0] and sigma = diag(1,1,...1)
    explicit MultiNormalDist(uint_t dimension);

    /// Constructor
    MultiNormalDist(const DynVec<real_t>& mu,
                    const DynMat<real_t>& sigma);

    /// \brief Calculate the pdf at the given point
    real_t pdf(const DynVec<real_t>& x);

    /// \brief Returns the dimensions
    uint_t get_dimension()const{return mu_.size();}


private:

    /// \brief The mean vector
    DynVec<real_t> mu_;

    /// \brief The covariance matrix
    DynMat<real_t> sigma_;

    /// \brief The inverse of the covariance
    DynMat<real_t> sigma_inv_;

    /// \brief The determinant of sigma
    real_t sigma_det_;
};

}
}

}

#endif // MULTIVARIATE_NORMAL_DISTRIBUTION_H
