#include "kernel/maths/statistics/multivariate_normal_distribution.h"
#include "kernel/maths/constants.h"

#include <stdexcept>
#include <cmath>

namespace kernel{
namespace maths{
namespace stats {

MultiNormalDist::MultiNormalDist(uint_t dimension)
    :
    mu_(dimension, 0.0),
    sigma_(dimension, dimension, 0.0),
    sigma_inv_(),
    sigma_det_(0.0)
{
    for(uint_t i=0; i<sigma_.rows(); ++i){
        sigma_(i,i) = 1.0;
    }
}

MultiNormalDist::MultiNormalDist(const DynVec<real_t>& mu,
                                 const DynMat<real_t>& sigma)
    :
      mu_(mu),
      sigma_(sigma),
      sigma_inv_(),
      sigma_det_(0.0)
{

    if(sigma_.rows() != mu_.size()){
        throw std::logic_error("Dimension mismatch for sigma and mean vector. "+
                               std::to_string(sigma_.rows()) +
                               " not equal to: "+
                               std::to_string(mu_.size()));
    }
}

real_t
MultiNormalDist::pdf(const DynVec<real_t>& x){

    if(x.size() != mu_.size()){
        throw std::logic_error("Dimension mismatch for point and mean vector. "+
                               std::to_string(x.size()) +
                               " not equal to: "+
                               std::to_string(mu_.size()));
    }

    if(sigma_inv_.rows() == 0){
        sigma_inv_ = inv(sigma_);
        sigma_det_ = det(sigma_);
    }

    auto vec = x - mu_;

    return (1.0/std::sqrt(2.0*MathConsts::PI*sigma_det_))*std::exp(-0.5*(trans(vec)*sigma_inv_*vec));
}

}
}
}
