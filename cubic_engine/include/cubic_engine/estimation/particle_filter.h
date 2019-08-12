/***
 *
 * Implementation of the Particle Filter algorithm.
 *  The algorithm implemented is described:
 *
 *
 *
 **/

#ifndef PARTICLE_FILTER_H
#define PARTICLE_FILTER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include <vector>
#include <exception>
#include <cassert>

namespace cengine
{

/// \brief Implements the Particle filter algorithm
class ParticleFilter
{

public:

    /// \brief Constructor. Constructs all particles having the same weight
    ParticleFilter(uint_t num_particles, real_t weight = static_cast<real_t>(1.0));

    /// \brief Set the weights for the particles
    void set_weights(const std::vector<real_t>& weights);


private:

    /// \brief number of particles to use
    uint_t num_particles_;

    /// \brief Vector of weights for all particles
    std::vector<real_t> weights_;

};


inline
void
ParticleFilter::set_weights(const std::vector<real_t>& weights){

    if(weights.size() != weights_.size()){
        throw std::runtime_error("Invalid number of weights");
    }

    weights_ = weights;
}


}

#endif // PARTICLE_FILTER_H
