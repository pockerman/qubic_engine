#ifndef MPC_CONTROL_H
#define MPC_CONTROL_H

#include "boost/noncopyable.hpp"

namespace cengine {
namespace control{


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


};

}

}

#endif // MPC_CONTROL_H
