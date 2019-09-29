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

#include <boost/noncopyable.hpp>
#include <vector>
#include <exception>
#include <cassert>
#include <map>

namespace cengine
{

/// \brief Helper class to provide a basic API for
/// matrix structure description for applications that use ParticleFilter class
class ParticleFilterMatrixDescriptor: private boost::noncopyable
{

public:

    /// \brief The matrix type this desciptor supports
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
class ParticleFilter: private boost::noncopyable
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
    ParticleFilter(state_type& state, uint_t num_particles,
                   uint_t num_particles_resample, real_t weight = static_cast<real_t>(1.0));


    /// \brief Set the weights for the particles
    void set_weights(const std::vector<real_t>& weights);


    /// \brief apply the particle filter using the given input
    void iterate(const input_type& input);


    /// \brief Prediction step
    void predict(const input_type& input);


    /// \brief Update step. Essentially performs the resampling step
    /// meaning that a new set of particles and particle weights is generated
    void update(const input_type& input);


    /// \brief Returns the number of particles used
    uint_t n_particles()const{return num_particles_;}

protected:


    /// \brief Normalize the weights
    virtual void normalize_weights();

private:


    /// \brief The state vector to estimate
    state_type& state_;


    /// \brief number of particles to use
    uint_t num_particles_;


    /// \brief number of particles for resampling
    uint_t num_particles_resample_;


    /// \brief the particles
    std::vector<std::vector<real_t>> particles_;


    /// \brief Vector of weights for all particles
    std::vector<real_t> weights_;


    /// \brief Model for generating the likelihood
    likelihood_model_type likelihood_;


    /// \brief The object that provides the matrix structure of the problem
    matrix_description_type matrix_descriptor_;


    /// \brief Resample
    void resample_(){}
};

template<typename StateTp, typename InputTp, typename MotionTp, typename MeasurementTp, typename LikelihoodModelTp, typename MatrixDescriptorTp>
ParticleFilter<StateTp, InputTp, MotionTp,
               MeasurementTp, LikelihoodModelTp, MatrixDescriptorTp>::ParticleFilter(StateTp& state, uint_t num_particles,
                                                                                     uint_t num_particles_resample, real_t weight)
    :
   state_(state),
   num_particles_(num_particles),
   num_particles_resample_(num_particles_resample),
   particles_(),
   weights_(num_particles, weight)
{

    // we need a particle for ever state variable
    particles_.resize(state.size(), std::vector<real_t>(num_particles, 0.0));
}

template<typename StateTp, typename InputTp, typename MotionTp, typename MeasurementTp, typename LikelihoodModelTp, typename MatrixDescriptorTp>
void
ParticleFilter<StateTp, InputTp, MotionTp,
               MeasurementTp, LikelihoodModelTp, MatrixDescriptorTp>::set_weights(const std::vector<real_t>& weights){

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

    predict(input);
    update(input);
}

template<typename StateTp, typename InputTp, typename MotionTp,
         typename MeasurementTp, typename LikelihoodModelTp, typename MatrixDescriptorTp>
void
ParticleFilter<StateTp, InputTp, MotionTp,
               MeasurementTp, LikelihoodModelTp, MatrixDescriptorTp>::predict(const typename ParticleFilter<StateTp, InputTp, MotionTp,
                                                                              MeasurementTp, LikelihoodModelTp, MatrixDescriptorTp>::input_type& input){

    typedef typename InputTp::control_type control_type;

    // get the control applied
    const auto& u = input.get<control_type>();

    for(uint_t p=0; p<n_particles(); ++p){

       # get the particle weight
       auto wp = weights_[p];

       # calculate state vector from motion model
       state_ = motion_model_(state_, matrix_descriptor_.get_matrix("F"), matrix_descriptor_.get_matrix("B"), u);

       kwargs['state'] = state_vec
       wp = self._importance_weight_calculator(wp, **kwargs)
       self._particles[:, pidx] = state_vec[:, 0]
       weights_[p] = wp
    }

    normalize_weights();

    state_vec = self._particles.dot(self._weights.T)
    self._mat_desc["P"] = self._covariance_calculator(state_vec, self._particles, self._weights)
    self.state = state_vec

    }

template<typename StateTp, typename InputTp, typename MotionTp,
         typename MeasurementTp, typename LikelihoodModelTp, typename MatrixDescriptorTp>
void
ParticleFilter<StateTp, InputTp, MotionTp,
               MeasurementTp, LikelihoodModelTp, MatrixDescriptorTp>:: update(const typename ParticleFilter<StateTp, InputTp, MotionTp,
                                                                              MeasurementTp, LikelihoodModelTp, MatrixDescriptorTp>::input_type& input){

    Neff = 1.0 / (self._weights.dot(self._weights.T))[0, 0]  # Effective particle number
           NP = self._num_particles
           NTh = self._num_resample_particles

           if Neff < NTh:
               wcum = np.cumsum(self._weights)
               base = np.cumsum(self._weights * 0.0 + 1 / NP) - 1 / NP
               resampleid = base + np.random.rand(base.shape[0]) / NP

               inds = []
               ind = 0
               for ip in range(NP):
                   while resampleid[ip] > wcum[ind]:
                       ind += 1
                   inds.append(ind)

               self._particles = self._particles[:, inds]
               self._weights = np.zeros((1, NP)) + 1.0 / NP  # init weight


}

}

#endif // PARTICLE_FILTER_H
