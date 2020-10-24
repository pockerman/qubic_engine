#include "cubic_engine/estimation/particle_filter.h"

namespace cengine
{
ParticleFilter::ParticleFilter(uint_t num_particles, real_t weight)
    :
   num_particles_(num_particles),
   weights_(num_particles, weight)
{}

}
