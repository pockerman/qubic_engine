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
#include <map>

namespace cengine
{

class ParticleFilterMatrixDescriptor
{

public:

    typedef DynMat<real_t> matrix_type;


    /// \brief Constructor
    ParticleFilterMatrixDescriptor();


    /// \brief Destructor
    virtual ~ParticleFilterMatrixDescriptor(){}


    /// \brief Return the names of the matrics involved
    /// in the definition of the Kalman Filter
    const std::vector<std::string> get_names()const;


    /// \brief Set the matrix with the given name
    void set_matrix(const std::string& name, matrix_type& matrix);


    /// \brief get_matrix. Returns read reference to the matrix_name matrix
    const matrix_type* get_matrix(const std::string& matrix_name)const;


    /// \brief get_matrix. Returns read reference to the matrix_name matrix
    matrix_type* get_matrix(const std::string& matrix_name);


    /// \brief Update the matrix descriptors
    virtual void update(){}

protected:


    /// \brief Pointers to the matrices describing the system
    std::map<std::string, matrix_type* > matrix_maps_;

};

/// \brief Implements the Particle filter algorithm
template<typename StateTp, typename InputTp, typename MotionTp, typename MeasurementTp, typename LikelihoodModelTp, typename MatrixDescriptorTp>
class ParticleFilter
{

public:

    typedef StateTp state_type;
    typedef InputTp input_type;
    typedef typename input_type::control_type control_type;
    typedef typename input_type::measurement_type measurement_type;
    typedef MatrixDescriptorTp matrix_description_type;
    typedef typename  matrix_description_type::matrix_type matrix_type;
    typedef LikelihoodModelTp likelihood_model_type;

    /// \brief Constructor. Constructs all particles having the same weight
    ParticleFilter(uint_t num_particles, uint_t num_particles_resample, real_t weight = static_cast<real_t>(1.0));


    /// \brief Set the weights for the particles
    void set_weights(const std::vector<real_t>& weights);


    /// \brief apply the particle filter using the given input
    void iterate(const input_type& input);


private:

    /// \brief number of particles to use
    uint_t num_particles_;

    /// \brief number of particles for resampling
    uint_t num_particles_resample_;

    /// \brief Vector of weights for all particles
    std::vector<real_t> weights_;


    /// \brief Model for generating the likelihood
    likelihood_model_type likelihood_;


    /// \brief Resample
    void resample_(){}
};

template<typename StateTp, typename InputTp, typename MotionTp, typename MeasurementTp, typename LikelihoodModelTp, typename MatrixDescriptorTp>
ParticleFilter<StateTp, InputTp, MotionTp, MeasurementTp, LikelihoodModelTp, MatrixDescriptorTp>::ParticleFilter(uint_t num_particles, uint_t num_particles_resample, real_t weight)
    :
   num_particles_(num_particles),
   num_particles_resample_(num_particles_resample),
   weights_(num_particles, weight)
{}

template<typename StateTp, typename InputTp, typename MotionTp, typename MeasurementTp, typename LikelihoodModelTp, typename MatrixDescriptorTp>
void
ParticleFilter<StateTp, InputTp, MotionTp, MeasurementTp, LikelihoodModelTp, MatrixDescriptorTp>::set_weights(const std::vector<real_t>& weights){

    if(weights.size() != weights_.size()){
        throw std::runtime_error("Invalid number of weights");
    }

    weights_ = weights;
}

template<typename StateTp, typename InputTp, typename MotionTp,
         typename MeasurementTp, typename LikelihoodModelTp, typename MatrixDescriptorTp>
void
ParticleFilter<StateTp, InputTp, MotionTp,
               MeasurementTp, LikelihoodModelTp, MatrixDescriptorTp>::iterate(const typename ParticleFilter<StateTp, InputTp, MotionTp,
                                                                              MeasurementTp, LikelihoodModelTp, MatrixDescriptorTp>::input_type& input){

    // loop over the particles
    for(uint_t p=0; p<num_particles_; ++p){

        //get the weight that corresponds for this particle
        real_t pw = weights_[p];

    }

    resample_();
}


}

#endif // PARTICLE_FILTER_H
