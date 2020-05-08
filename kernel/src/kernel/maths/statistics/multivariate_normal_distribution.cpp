#include "kernel/maths/statistics/multivariate_normal_distribution.h"
#include "kernel/maths/constants.h"

#include <stdexcept>
#include <random>
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
MultiNormalDist::pdf(const DynVec<real_t>& x)const{

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


DynVec<real_t>
MultiNormalDist::sample()const{

    DynVec<real_t> x(mu_.size(), 0.0);

    /// the white noise for the observation
    std::normal_distribution<real_t> distribution;
    std::random_device rd;
    std::mt19937 gen(rd());

    for(uint_t d=0; d<x.size(); ++d){
        x[d] = distribution(gen);
    }

    /// now transform to multivariate by using
    /// y = L^{1/2}Qx + mu
    /// where
    /// L: a diagonal matrix with the eigenvalues of Sigma
    /// Q: Matrix with the eignevectors of Sigma as columns

    /// Note that L is row major so the eigenvectors
    /// are returned as rows...so we need to to transpose
    /// below
    DynMat<std::complex<real_t>> V(sigma_.rows(), sigma_.rows(), 0.0);
    DynVec<std::complex<real_t>> w(sigma_.rows(), 0.0);

    /// compute eigenvalues and eigenvectors
    eigen(sigma_, w, V);

    DynMat<real_t> L(sigma_.rows(), sigma_.rows(), 0.0);
    DynMat<real_t> Q(sigma_.rows(), sigma_.rows(), 0.0);

    for(uint_t r=0; r<sigma_.rows(); ++r ){
        L(r,r) = w[r].real();

        for(uint_t c=0; c<sigma_.rows(); ++c ){
            Q(r,c) = V(r,c).real();
        }
    }

    L = sqrt(L);
    return L*trans(Q)*x + mu_;
}

}
}
}
