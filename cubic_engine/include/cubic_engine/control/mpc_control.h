#ifndef MPC_CONTROL_H
#define MPC_CONTROL_H

#include "boost/noncopyable.hpp"

namespace cengine {
namespace control{


///
/// \brief The MPCInput struct. Wrap the input
/// required by the the MPCController class
///
struct MPCInput
{

};

///
/// \brief The MPCController class. Linear constrained MPC controller
///
template<typename OptimizerTp, typename ObserverTp, typename PredictorTp>
class MPCController: private boost::noncopyable
{

public:

    ///
    /// \brief optimizer_t The type of the optimizer
    ///
    typedef OptimizerTp optimizer_t;

    ///
    /// \brief control_output_t The output the MPC provides
    /// to the calling application
    ///
    typedef typename optimizer_t::output_t control_output_t;

    ///
    /// \brief observer_t The type of the observer
    ///
    typedef ObserverTp observer_t;

    ///
    /// \brief predictor_t The type of the predictor
    ///
    typedef PredictorTp predictor_t;

    ///
    /// \brief MPCController. Constructor
    ///
    MPCController(const MPCInput&);

    ///
    /// \brief update. Update the controller
    ///
    void update();

    ///
    /// \brief solve. Solve the optimization problem
    ///
    void solve();

    ///
    /// \brief control_value Returns the control value computed
    /// by the controller to be passed to the application
    ///
    control_output_t control_value()const;

private:

    ///
    /// \brief input_
    ///
    MPCInput input_;

    ///
    /// \brief optimizer_ The optimizer to use
    ///
    optimizer_t optimizer_;

};

template<typename OptimizerTp, typename ObserverTp, typename PredictorTp>
MPCController<OptimizerTp, ObserverTp, PredictorTp>::MPCController(const MPCInput& in)
    :
      input_(in),
      optimizer_()
{}


template<typename OptimizerTp, typename ObserverTp, typename PredictorTp>
void
MPCController<OptimizerTp, ObserverTp, PredictorTp>::update(){

}

template<typename OptimizerTp, typename ObserverTp, typename PredictorTp>
void
MPCController<OptimizerTp, ObserverTp, PredictorTp>::solve(){

}

template<typename OptimizerTp, typename ObserverTp, typename PredictorTp>
typename MPCController<OptimizerTp, ObserverTp, PredictorTp>::control_output_t
MPCController<OptimizerTp, ObserverTp, PredictorTp>::control_value()const{

}

}

}

#endif // MPC_CONTROL_H
