#ifndef MPC_CONTROL_H
#define MPC_CONTROL_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/input_resolver.h"
#include "kernel/maths/optimization/quadratic_problem.h"

#include <boost/noncopyable.hpp>
#include <boost/any.hpp>
#include <map>
#include <string>


namespace cengine {
namespace control{


///
/// \brief The MPCInput struct. Wrap the input
/// required by the the MPCController class
///
template<typename OptimizerTp, typename ObserverTp, typename EstimatorTp>
struct MPCConfig
{
    ///
    /// \brief optimizer_t The type of the optimizer
    ///
    typedef OptimizerTp optimizer_t;

    ///
    /// \brief optimizer_config_t Configuration type of the optimizer
    ///
    typedef typename optimizer_t::config_t optimizer_config_t;

    ///
    /// \brief observer_t The type of the observer
    ///
    typedef ObserverTp observer_t;

    ///
    /// \brief observer_config_t Configuration type of the observer
    ///
    typedef typename  observer_t::config_t observer_config_t;

    ///
    /// \brief predictor_t The type of the predictor
    ///
    typedef EstimatorTp estimator_t;

    ///
    /// \brief predictor_config_t Configuration type of the predictor
    ///
    typedef typename estimator_t::config_t estimator_config_t;

    ///
    /// \brief vector_t Type of vector
    ///
    typedef typename optimizer_t::vector_t vector_t;

    ///
    /// \brief matrix_t Type of matrix
    ///
    typedef typename optimizer_t::matrix_t matrix_t;

    ///
    /// \brief opt_config The configuration of the optimizer
    ///
    optimizer_config_t opt_config;

    ///
    /// \brief obs_config The configuration of the observer
    ///
    observer_config_t obs_config;

    ///
    /// \brief pred_config The configuration of the predictor
    ///
    estimator_config_t estimator_config;

    ///
    /// \brief x_ref Reference state
    ///
    vector_t x_ref;

    ///
    /// \brief x0 The initial state
    ///
    vector_t x0;

    ///
    /// \brief x_previous The previous state solution
    /// if not set then it is set equal to x0 upon
    /// the setup function of the controller
    ///
    vector_t x_previous;

    ///
    /// \brief Np The prediction horizon
    ///
    uint_t Np;

    ///
    /// \brief Nu The control horizon
    ///
    uint_t Nu;

};

///
/// \brief The MPCController class. Linear constrained MPC controller
///
template<typename OptimizerTp, typename ObserverTp, typename EstimatorTp>
class MPCController: private boost::noncopyable
{

public:

    ///
    /// \brief optimizer_t The type of the optimizer
    ///
    typedef OptimizerTp optimizer_t;

    ///
    /// \brief observer_t The type of the observer
    ///
    typedef ObserverTp observer_t;

    ///
    /// \brief predictor_t The type of the predictor
    ///
    typedef EstimatorTp estimator_t;

    ///
    /// \brief config_t The type of the configuration
    ///
    typedef MPCConfig<optimizer_t, observer_t, estimator_t> config_t;

    ///
    /// \brief control_output_t The output the MPC provides
    /// to the calling application
    ///
    typedef typename optimizer_t::output_t control_output_t;

    ///
    /// \brief input_t Input to the controller
    ///
    typedef std::map<std::string, boost::any> input_t;

    ///
    /// \brief vector_t Type of vector
    ///
    typedef typename optimizer_t::vector_t vector_t;

    ///
    /// \brief matrix_t Type of matrix
    ///
    typedef typename optimizer_t::matrix_t matrix_t;

    ///
    /// \brief quadratic_t The type of the quadratic problem to solve
    ///
    typedef kernel::maths::opt::QuadraticProblem<matrix_t, vector_t> quadratic_t;

    ///
    /// \brief MPCController. Constructor
    ///
    MPCController(const config_t& config);

    ///
    /// \brief get_qp Returns read/write reference to the quadratic problem
    /// to be solved
    ///
    quadratic_t& get_qp(){return qp_;}

    ///
    /// \brief get_qp Returns read reference to the quadratic problem
    /// to be solved
    ///
    const quadratic_t& get_qp()const{return qp_;}

    ///
    /// \brief update. Update the controller
    ///
    void update(const vector_t& state, const vector_t* state_ref=nullptr);

    ///
    /// \brief solve. Solve the optimization problem
    ///
    void solve(const input_t& input);

    ///
    /// \brief control_value Returns the control value computed
    /// by the controller to be passed to the application
    ///
    const control_output_t& control_output()const{return control_out_;}

private:

    ///
    /// \brief input_
    ///
    config_t config_;

    ///
    /// \brief optimizer_ The optimizer to use
    ///
    optimizer_t optimizer_;

    ///
    /// \brief observer_ The type of the observer
    ///
    observer_t observer_;

    ///
    /// \brief predictor_ The type of the predictor
    ///
    estimator_t estimator_;

    ///
    /// \brief qp The quadratic problem to solve for
    ///
    quadratic_t qp_;

    ///
    /// \brief control_out_ The output we update every
    /// time the solve method is called
    ///
    control_output_t control_out_;

};

template<typename OptimizerTp, typename ObserverTp, typename EstimatorTp>
MPCController<OptimizerTp, ObserverTp, EstimatorTp>::MPCController(const config_t& config)
    :
      config_(config),
      optimizer_(config.opt_config),
      observer_(config.obs_config),
      estimator_(config.estimator_config)
{}


template<typename OptimizerTp, typename ObserverTp, typename EstimatorTp>
void
MPCController<OptimizerTp, ObserverTp, EstimatorTp>::update(const vector_t& state, const vector_t* state_ref){

    config_.x_previous = state;

    // update the reference state if needed
    if(state_ref){
        config_.x_ref = *state_ref;
    }

}

template<typename OptimizerTp, typename ObserverTp, typename EstimatorTp>
void
MPCController<OptimizerTp, ObserverTp, EstimatorTp>::solve(const input_t& input){

    // input for the estimator
    auto estimator_in = kernel::utils::InputResolver<input_t,
            typename EstimatorTp::input_t>::resolve("estimator_input", input);

    // solve over the prediction

    for(uint_t itr=0; itr<config_.Np; ++itr){

        // state estimation
        estimator_.estimate(estimator_in);

        // get the state from the estimator
        auto& state = estimator_.get_state();

        // form the difference between
        // state vector and state reference
        qp_.x = state.as_vector() -  config_.x_ref;

        // optimizer solve the quadratic problem
        optimizer_.solve(qp_);

        // we can now return the control inputs
    }

}

}

}

#endif // MPC_CONTROL_H
