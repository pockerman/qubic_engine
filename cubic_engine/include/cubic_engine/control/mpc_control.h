#ifndef MPC_CONTROL_H
#define MPC_CONTROL_H

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
template<typename OptimizerTp, typename ObserverTp, typename PredictorTp>
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
    typedef PredictorTp predictor_t;

    ///
    /// \brief predictor_config_t Configuration type of the predictor
    ///
    typedef typename predictor_t::config_t predictor_config_t;

    ///
    /// \brief vector_t Type of vector
    ///
    typedef typename OptimizerTp::vector_t vector_t;

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
    predictor_config_t pred_config;

    ///
    /// \brief x_ref Reference state
    ///
    vector_t x_ref;

    ///
    /// \brief min min constraints
    ///
    vector_t min;

    ///
    /// \brief max max constraints
    ///
    vector_t max;

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
    /// \brief MPCController. Constructor
    ///
    MPCController(const config_t& config);

    ///
    /// \brief update. Update the controller
    ///
    void update();

    ///
    /// \brief solve. Solve the optimization problem
    ///
    void solve(const input_t& input);

    ///
    /// \brief control_value Returns the control value computed
    /// by the controller to be passed to the application
    ///
    control_output_t control_value()const;

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

};

template<typename OptimizerTp, typename ObserverTp, typename PredictorTp>
MPCController<OptimizerTp, ObserverTp, PredictorTp>::MPCController(const config_t& config)
    :
      config_(config),
      optimizer_(config.opt_config),
      observer_(config.obs_config),
      estimator_(config.pred_config)
{}


template<typename OptimizerTp, typename ObserverTp, typename PredictorTp>
void
MPCController<OptimizerTp, ObserverTp, PredictorTp>::update(){

}

template<typename OptimizerTp, typename ObserverTp, typename PredictorTp>
void
MPCController<OptimizerTp, ObserverTp, PredictorTp>::solve(const input_t& input){

    // input for the estimator
    auto estimator_in = kernel::utils::InputResolver<input_t, std::string>::resolve("estimator_input", input);

    // state estimation
    estimator_.estimate(estimator_in);


    kernel::maths::opt::QuadraticProblem<typename OptimizerTp::matrix_t,
                                         typename OptimizerTp::vector_t> qp;

    // optimizer solve the quadratic problem
    optimizer_.solve(qp);

}

template<typename OptimizerTp, typename ObserverTp, typename PredictorTp>
typename MPCController<OptimizerTp, ObserverTp, PredictorTp>::control_output_t
MPCController<OptimizerTp, ObserverTp, PredictorTp>::control_value()const{

}

}

}

#endif // MPC_CONTROL_H
