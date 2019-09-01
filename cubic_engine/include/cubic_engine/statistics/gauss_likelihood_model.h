#ifndef GAUSS_LIKELIHOOD_MODEL_H
#define GAUSS_LIKELIHOOD_MODEL_H

#include "cubic_engine/base/cubic_engine_types.h"
#include <random>

namespace cengine
{

template<typename MeanTp, typename SigmaTp>
class GaussLikelihood
{

public:


    typedef MeanTp mean_type;
    typedef SigmaTp sigma_type;
    typedef mean_type result_type;

    /// \brief Constructor
    GaussLikelihood();


    /// \brief Constructor
    GaussLikelihood(const mean_type& mu, const sigma_type& sigma);


    /// \brief set the mean
    void set_mean(const mean_type& mu){mu_ = mu;}


    /// \brief set the sigma
    void set_sigma(const sigma_type& sigma){sigma_ = sigma;}

    /// \brief Returns a value from the normal distribution
    result_type operator()()const;

private:

    mean_type mu_;
    sigma_type sigma_;
};


template<typename MeanTp, typename SigmaTp>
GaussLikelihood<MeanTp, SigmaTp>::GaussLikelihood()
    :
mu_(0.0),
sigma_(1.0)
{}



template<typename MeanTp, typename SigmaTp>
GaussLikelihood<MeanTp, SigmaTp>::GaussLikelihood(const typename GaussLikelihood<MeanTp, SigmaTp>::mean_type& mu,
                                                  const typename GaussLikelihood<MeanTp, SigmaTp>::sigma_type& sigma)
    :
mu_(mu),
sigma_(sigma)
{}

template<typename MeanTp, typename SigmaTp>
typename GaussLikelihood<MeanTp, SigmaTp>::result_type
GaussLikelihood<MeanTp, SigmaTp>::operator()()const{

    std::default_random_engine generator;
    std::normal_distribution<real_t> dist(mu_, sigma_);
    return dist(generator);
}

}

#endif // GAUSS_LIKELIHOOD_MODEL_H
